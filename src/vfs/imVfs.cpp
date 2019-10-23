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
#include <string_theory/stdio>

// Cross-Platform filesystem stuff
#ifdef WIN32

    #include <windows.h>
    #include <sys/types.h>
    #include <sys/stat.h>

    ST::string path_filename(const ST::string& path)
    {
        ST_ssize_t split = path.find_last('\\');
        if (split == -1)
            return path;
        return path.substr(split + 1);
    }

    ST::string path_pathname(const ST::string& path)
    {
        ST_ssize_t split = path.find_last('\\');
        if (split == -1)
            return ST::string();
        return path.left(split);
    }

    bool file_exists(const ST::string& path)
    {
        struct _stat stbuf;
        return _wstat(path.to_wchar().data(), &stbuf) == 0;
    }

    static std::shared_ptr<imVfsEntry> find_files(const ST::string& path)
    {
        WIN32_FIND_DATAW fd;
        HANDLE hList = FindFirstFileW(path.to_wchar().data(), &fd);
        if (hList == 0)
            return 0;

        std::vector<std::shared_ptr<imVfsEntry>> children;
        do {
            if (wcscmp(fd.cFileName, L".") == 0 || wcscmp(fd.cFileName, L"..") == 0)
                continue;
            if ((fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0) {
                // Add an imVfsDirEntry
                children.push_back(find_files(path + "\\" + fd.cFileName));
            } else {
                // Add regular files
                children.push_back(
                    std::make_shared<imVfsFileEntry>(fd.cFileName,
                            imVfsEntry::Type_File, fd.nFileSizeLow,
                            path + "\\" + fd.cFileName)
                );
            }
        } while (FindNextFileW(hList, &fd));
        FindClose(hList);
        return std::make_shared<imVfsDirEntry>(path_filename(path), children);
    }

#else

    #include <unistd.h>
    #include <dirent.h>
    #include <sys/types.h>
    #include <sys/stat.h>

    ST::string path_filename(const ST::string& path)
    {
        ST_ssize_t split = path.find_last('/');
        if (split == -1)
            return path;
        return path.substr(split + 1);
    }

    ST::string path_pathname(const ST::string& path)
    {
        ST_ssize_t split = path.find_last('/');
        if (split == (size_t)-1)
            return ST::string();
        return path.left(split);
    }

    bool file_exists(const ST::string& path)
    {
        struct stat stbuf;
        return stat(path.c_str(), &stbuf) == 0;
    }

    static std::shared_ptr<imVfsEntry> find_files(const ST::string& path)
    {
        dirent** des;
        int count = scandir(path.c_str(), &des, 0, 0);
        if (count < 0)
            return 0;

        std::vector<std::shared_ptr<imVfsEntry>> children;
        children.reserve(static_cast<size_t>(count));
        for (int i=0; i<count; ++i) {
            if (strcmp(des[i]->d_name, ".") == 0 || strcmp(des[i]->d_name, "..") == 0) {
                // Ignore these entries
            } else if (des[i]->d_type == DT_DIR) {
                // Add an imVfsDirEntry
                children.push_back(find_files(path + "/" + des[i]->d_name));
            } else {
                // Add regular files
                struct stat fs;
                const ST::string file_path = path + "/" + des[i]->d_name;
                if (stat(file_path.c_str(), &fs) == 0) {
                    children.push_back(
                        std::make_shared<imVfsFileEntry>(des[i]->d_name,
                                imVfsEntry::Type_File,fs.st_size, file_path)
                    );
                } else {
                    // TODO: Don't fail silently!
                }
            }
            free(des[i]);
        }
        free(des);
        return std::make_shared<imVfsDirEntry>(path_filename(path), children);
    }

#endif
// End file listing code


/* imVfsDirEntry */
std::vector<std::shared_ptr<imVfsEntry>> imVfsDirEntry::children() const
{
    std::vector<std::shared_ptr<imVfsEntry>> result;
    result.reserve(m_children.size());
    for (const auto& child : m_children)
        result.push_back(child.second);
    return result;
}

std::shared_ptr<imVfsEntry> imVfsDirEntry::get(const ST::string& name) const
{
    auto f = m_children.find(name);
    if (f == m_children.end())
        return nullptr;
    return f->second;
}

void imVfsDirEntry::add(const std::vector<std::shared_ptr<imVfsEntry>>& children)
{
    for (const auto &child : children) {
        auto f = m_children.find(child->name());
        if (f == m_children.end()) {
            m_children[child->name()] = child;
        } else if (f->second->isDirectory() && child->isDirectory()) {
            // Merge the contents of the directory with the new location
            std::static_pointer_cast<imVfsDirEntry>(f->second)->add(
                std::static_pointer_cast<imVfsDirEntry>(child)->children()
            );
        } else {
            imLog("WARN: Duplicate VFS entry '{}'", child->name());
            m_children[child->name()] = child;
        }
    }
}


/* imVfs */
imVfs::~imVfs()
{
    for (const auto &it : m_dniStreams)
        delete it.second;
}

void imVfs::addPhysicalPath(const ST::string& path)
{
    std::shared_ptr<imVfsEntry> dir = find_files(path);
    if (!dir)
        return;

    if (dir->isDirectory()) {
        m_root->add(std::static_pointer_cast<imVfsDirEntry>(dir)->children());
    } else {
        std::vector<std::shared_ptr<imVfsEntry>> file = {dir};
        m_root->add(file);
    }
}

void imVfs::addDniFile(const ST::string& filename)
{
    const ST::string location = path_filename(filename);
    std::unique_ptr<imFileStream> stream(new imFileStream);
    if (!stream->open(filename, "rb"))
        return;

    std::shared_ptr<imVfsEntry> dir = read_dnifile(stream.get(), location);
    if (!dir)
        return;

    if (dir->isDirectory()) {
        m_root->add(std::static_pointer_cast<imVfsDirEntry>(dir)->children());
    } else {
        std::vector<std::shared_ptr<imVfsEntry>> file = {dir};
        m_root->add(file);
    }
    m_dniStreams[location] = stream.release();
}

static void print_recur(imVfsEntry* ent, int tab)
{
    for (int i=0; i<tab; ++i)
        printf("    ");
    if (ent->isDirectory()) {
        ST::printf("[{}]\n", ent->name());
        const auto& children = static_cast<imVfsDirEntry*>(ent)->children();
        for (const auto& child : children)
            print_recur(child.get(), tab + 1);
    } else {
        ST::printf("{}\n", ent->name());
    }
}

void imVfs::debug_print()
{
    print_recur(m_root.get(), 0);
}

imStream* imVfs::open(const ST::string& path)
{
    if (path[0] != '/')
        return nullptr;
    ST::string open_path = path.substr(1);

    auto dir = m_root;
    do {
        if (!dir || !dir->isDirectory())
            return nullptr;

        ST_ssize_t split = open_path.find('/');
        if (split == -1) {
            // Last path item, this should be what we want
            auto entry = std::static_pointer_cast<imVfsFileEntry>(dir->get(open_path));
            if (!entry)
                return nullptr;

            if (entry->type() == imVfsEntry::Type_File) {
                imFileStream* stream = new imFileStream();
                if (stream->open(entry->location(), "rb"))
                    return stream;
                delete stream;
                return nullptr;
            } else if (entry->type() == imVfsEntry::Type_VFile) {
                imStream* dni = m_dniStreams[entry->location()];
                dni->seek(entry->voffset());
                std::unique_ptr<unsigned char[]> buffer(new unsigned char[entry->size()]);
                if (entry->isCompressed()) {
                    std::unique_ptr<unsigned char[]> zbuf(new unsigned char[entry->csize()]);
                    dni->read(zbuf.get(), entry->csize());
                    uLongf bufLen = entry->size();
                    if (uncompress(buffer.get(), &bufLen, zbuf.get(), entry->csize()) != Z_OK)
                        return nullptr;
                } else {
                    dni->read(buffer.get(), entry->size());
                }
                return new imBufferStream(buffer.release(), entry->size(), true);
            } else {
                // Can't read from a directory
                return nullptr;
            }
        } else {
            // Directory along our way
            ST::string subdir = open_path.left(split);
            open_path = open_path.substr(split + 1);
            dir = std::static_pointer_cast<imVfsDirEntry>(dir->get(subdir));
        }
    } while (!open_path.empty());
    return nullptr;
}
