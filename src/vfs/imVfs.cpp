#include "imVfs.h"
#include <zlib.h>
#include <cstring>
#include <cstdio>

// Cross-Platform magic for listing filesystem files
#ifdef WIN32

    #include <windows.h>

    imString path_filename(imString path)
    {
        size_t split = path.r_find('\\');
        if (split == (size_t)-1)
            return path;
        return path.mid(split + 1);
    }

    static imVfsEntry* find_files(imString path)
    {
        WIN32_FIND_DATAA fd;
        HANDLE hList = FindFirstFileA(path.cstr(), &fd);
        if (hList == 0)
            return 0;

        std::list<imRef<imVfsEntry> > children;
        do {
            if (strcmp(fd.cFileName, ".") == 0 || strcmp(fd.cFileName, "..") == 0)
                continue;
            if ((fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECOTRY) != 0) {
                // Add an imVfsDirEntry
                children.push_back(find_files(path + "\\" + fd.cFileName));
            } else {
                // Add regular files
                children.push_back(
                    new imVfsFileEntry(fd.cFileName, imVfsEntry::Type_File,
                                       fd.nFileSizeLow, path + "\\" + fd.cFileName)
                );
            }
        } while (FindNextFile(hList, &fd));
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
            fprintf(stderr, "WARN: Duplicate VFS entry '%s'\n", ipath.data());
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
    if (!path[0] == '/')
        return 0;
    path = path.mid(1);

    imRef<imVfsDirEntry> dir = m_root;
    do {
        if (dir == 0 || !dir->isDirectory())
            return 0;

        size_t split = path.find('/');
        if (split == (size_t)-1) {
            // Last path item, this should be what we want
            imVfsFileEntry* entry = dir->get(path).cast<imVfsFileEntry>();
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
            dir = dir->get(subdir).cast<imVfsDirEntry>();
        }
    } while (!path.isEmpty());
    return 0;
}
