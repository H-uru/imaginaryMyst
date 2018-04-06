/* This file is part of imaginaryMyst.
 *
 * imaginaryMyst is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * imaginaryMyst is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with imaginaryMyst.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "imVfs.h"
#include "imCommon.h"
#include <zlib.h>
#include <cstring>

// Cross-Platform filesystem stuff
#ifdef WIN32

    #include <windows.h>
    #include <sys/types.h>
    #include <sys/stat.h>

    imString path_filename(imString path)
    {
        size_t split = path.r_find('\\');
        if (split == (size_t)-1)
            return path;
        return path.mid(split + 1);
    }

    imString path_pathname(imString path)
    {
        size_t split = path.r_find('\\');
        if (split == (size_t)-1)
            return "";
        return path.left(split);
    }

    bool file_exists(imString path)
    {
        struct _stat stbuf;
        return _stat(path.data(), &stbuf) == 0;
    }

    static imVfsEntry* find_files(imString path)
    {
        WIN32_FIND_DATAA fd;
        HANDLE hList = FindFirstFileA(path.data(), &fd);
        if (hList == 0)
            return 0;

        std::list<imRef<imVfsEntry> > children;
        do {
            if (strcmp(fd.cFileName, ".") == 0 || strcmp(fd.cFileName, "..") == 0)
                continue;
            if ((fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0) {
                // Add an imVfsDirEntry
                children.push_back(find_files(path + "\\" + fd.cFileName));
            } else {
                // Add regular files
                children.push_back(
                    new imVfsFileEntry(fd.cFileName, imVfsEntry::Type_File,
                                       fd.nFileSizeLow, path + "\\" + fd.cFileName)
                );
            }
        } while (FindNextFileA(hList, &fd));
        FindClose(hList);
        return new imVfsDirEntry(path_filename(path), children);
    }

#else

    #include <unistd.h>
    #include <dirent.h>
    #include <sys/types.h>
    #include <sys/stat.h>

    imString path_filename(imString path)
    {
        size_t split = path.r_find('/');
        if (split == (size_t)-1)
            return path;
        return path.mid(split + 1);
    }

    imString path_pathname(imString path)
    {
        size_t split = path.r_find('/');
        if (split == (size_t)-1)
            return "";
        return path.left(split);
    }

    bool file_exists(imString path)
    {
        struct stat stbuf;
        return stat(path.data(), &stbuf) == 0;
    }

    static imVfsEntry* find_files(imString path)
    {
        dirent** des;
        int count = scandir(path.data(), &des, 0, 0);
        if (count < 0)
            return 0;

        std::list<imRef<imVfsEntry> > children;
        for (int i=0; i<count; ++i) {
            if (strcmp(des[i]->d_name, ".") == 0 || strcmp(des[i]->d_name, "..") == 0) {
                // Ignore these entries
            } else if (des[i]->d_type == DT_DIR) {
                // Add an imVfsDirEntry
                children.push_back(find_files(path + "/" + des[i]->d_name));
            } else {
                // Add regular files
                struct stat fs;
                if (stat(path + "/" + des[i]->d_name, &fs) == 0) {
                    children.push_back(
                        new imVfsFileEntry(des[i]->d_name, imVfsEntry::Type_File,
                                           fs.st_size, path + "/" + des[i]->d_name)
                    );
                } else {
                    // TODO: Don't fail silently!
                }
            }
            free(des[i]);
        }
        free(des);
        return new imVfsDirEntry(path_filename(path), children);
    }

#endif
// End file listing code


/* imVfsDirEntry */
std::list<imRef<imVfsEntry> > imVfsDirEntry::children() const
{
    std::list<imRef<imVfsEntry> > childList;
    std::map<imString, imRef<imVfsEntry> >::const_iterator it;
    for (it = m_children.begin(); it != m_children.end(); ++it)
        childList.push_back(it->second);
    return childList;
}

imRef<imVfsEntry> imVfsDirEntry::get(imString name) const
{
    name = name.toLower();
    std::map<imString, imRef<imVfsEntry> >::const_iterator f;
    f = m_children.find(name);
    if (f == m_children.end())
        return 0;
    return f->second;
}

void imVfsDirEntry::add(const std::list<imRef<imVfsEntry> >& children)
{
    std::list<imRef<imVfsEntry> >::const_iterator it;
    for (it = children.begin(); it != children.end(); ++it) {
        imString ipath = (*it)->name().toLower();
        std::map<imString, imRef<imVfsEntry> >::iterator f;
        f = m_children.find(ipath);
        if (f == m_children.end()) {
            m_children[ipath] = *it;
        } else if (f->second->isDirectory() && (*it)->isDirectory()) {
            // Merge the contents of the directory with the new location
            f->second.cast<imVfsDirEntry>()->add(
                it->cast<imVfsDirEntry>()->children()
            );
        } else {
            imLog("WARN: Duplicate VFS entry '%s'", ipath.data());
            m_children[ipath] = *it;
        }
    }
}


/* imVfs */
imVfs::~imVfs()
{
    std::map<imString, imStream*>::iterator it;
    for (it = m_dniStreams.begin(); it != m_dniStreams.end(); ++it)
        delete it->second;
}

void imVfs::addPhysicalPath(imString path)
{
    imRef<imVfsEntry> dir = find_files(path);
    if (dir == 0)
        return;

    if (dir->isDirectory()) {
        m_root->add(dir.cast<imVfsDirEntry>()->children());
    } else {
        std::list<imRef<imVfsEntry> > file;
        file.push_back(dir);
        m_root->add(file);
    }
}

void imVfs::addDniFile(imString filename)
{
    imString location = path_filename(filename);
    imFileStream* stream = new imFileStream();
    if (!stream->open(filename, "rb")) {
        delete stream;
        return;
    }

    imRef<imVfsEntry> dir = read_dnifile(stream, location);
    if (dir == 0) {
        delete stream;
        return;
    }

    if (dir->isDirectory()) {
        m_root->add(dir.cast<imVfsDirEntry>()->children());
    } else {
        std::list<imRef<imVfsEntry> > file;
        file.push_back(dir);
        m_root->add(file);
    }
    m_dniStreams[location] = stream;
}

static void print_recur(imVfsEntry* ent, int tab)
{
    for (int i=0; i<tab; ++i)
        printf("    ");
    if (ent->isDirectory()) {
        printf("[%s]\n", ent->name().data());
        std::list<imRef<imVfsEntry> > children = ((imVfsDirEntry*)ent)->children();
        for (std::list<imRef<imVfsEntry> >::iterator it = children.begin(); it != children.end(); ++it)
            print_recur(*it, tab + 1);
    } else {
        printf("%s\n", ent->name().data());
    }
}

void imVfs::debug_print()
{
    print_recur(m_root, 0);
}

imStream* imVfs::open(imString path)
{
    if (!(path[0] == '/'))
        return 0;
    path = path.mid(1);

    imRef<imVfsDirEntry> dir = m_root;
    do {
        if (dir == 0 || !dir->isDirectory())
            return 0;

        size_t split = path.find('/');
        if (split == (size_t)-1) {
            // Last path item, this should be what we want
            imRef<imVfsFileEntry> entry = dir->get(path).convert<imVfsFileEntry>();
            if (entry == 0)
                return 0;

            if (entry->type() == imVfsEntry::Type_File) {
                imFileStream* stream = new imFileStream();
                if (stream->open(entry->location(), "rb"))
                    return stream;
                delete stream;
                return 0;
            } else if (entry->type() == imVfsEntry::Type_VFile) {
                imStream* dni = m_dniStreams[entry->location()];
                dni->seek(entry->voffset());
                unsigned char* buffer = new unsigned char[entry->size()];
                if (entry->isCompressed()) {
                    unsigned char* zbuf = new unsigned char[entry->csize()];
                    dni->read(zbuf, entry->csize());
                    uLongf bufLen = entry->size();
                    if (uncompress(buffer, &bufLen, zbuf, entry->csize()) != Z_OK) {
                        delete[] buffer;
                        delete[] zbuf;
                        return 0;
                    }
                    delete[] zbuf;
                } else {
                    dni->read(buffer, entry->size());
                }
                return new imBufferStream(buffer, entry->size(), true);
            } else {
                // Can't read from a directory
                return 0;
            }
        } else {
            // Directory along our way
            imString subdir = path.left(split);
            path = path.mid(split + 1);
            dir = dir->get(subdir).convert<imVfsDirEntry>();
        }
    } while (!path.isEmpty());
    return 0;
}
