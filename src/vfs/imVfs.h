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

#ifndef _IM_VFS_H
#define _IM_VFS_H

#include "imStream.h"
#include <string_theory/format>
#include <map>
#include <vector>
#include <memory>

class imVfsEntry {
public:
    enum VfsType {
        Type_Directory, Type_File, Type_VFile,
        Type_Mask = 0x000F,

        Type_ZLib = 0x0010,
    };

public:
    imVfsEntry(ST::string name, VfsType type)
        : m_name(std::move(name)), m_type(type)
    { }

    virtual ~imVfsEntry() { }

    bool isDirectory() const
    {
        return ((m_type & Type_Mask) == Type_Directory);
    }

    bool isCompressed() const
    { return (m_type & Type_ZLib) != 0; }

    const ST::string& name() const
    { return m_name; }

    VfsType type() const
    { return (VfsType)(m_type & Type_Mask); }

    void setCompressed()
    { m_type = (VfsType)(m_type | Type_ZLib); }

private:
    ST::string m_name;
    VfsType m_type;
};


class imVfsDirEntry : public imVfsEntry {
public:
    imVfsDirEntry()
        : imVfsEntry({}, Type_Directory)
    { }

    imVfsDirEntry(ST::string name, const std::vector<std::shared_ptr<imVfsEntry>>& children)
        : imVfsEntry(std::move(name), Type_Directory)
    {
        add(children);
    }

    std::vector<std::shared_ptr<imVfsEntry>> children() const;
    std::shared_ptr<imVfsEntry> get(const ST::string& name) const;

    void add(const std::vector<std::shared_ptr<imVfsEntry>>& children);

private:
    // Map these by the entry name for faster lookups
    std::map<ST::string, std::shared_ptr<imVfsEntry>, ST::less_i> m_children;
};


class imVfsFileEntry : public imVfsEntry {
public:
    imVfsFileEntry(ST::string name, VfsType type, size_t size, ST::string location)
        : imVfsEntry(std::move(name), type), m_vext(), m_voffset(), m_size(size),
          m_csize(), m_location(std::move(location))
    { }

    imVfsFileEntry(ST::string name, VfsType type, unsigned int vext,
                   size_t offset, size_t size, size_t csize, ST::string location)
        : imVfsEntry(std::move(name), type), m_vext(vext), m_voffset(offset),
          m_size(size), m_csize(csize), m_location(std::move(location))
    {
        if (m_csize != 0)
            setCompressed();
    }

    unsigned int vext() const
    { return m_vext; }

    size_t voffset() const
    { return m_voffset; }

    size_t size() const
    { return m_size; }

    size_t csize() const
    { return m_csize; }

    const ST::string& location() const
    { return m_location; }

private:
    unsigned int m_vext;    // Still don't know what this is...
    size_t m_voffset;       // Offset in Virtual files
    size_t m_size;          // Size of file
    size_t m_csize;         // Compressed (Zlib) size
    ST::string m_location;  // Where the file is stored
};


class imVfs {
public:
    imVfs() : m_root(std::make_shared<imVfsDirEntry>()) { }
    ~imVfs();

    void addPhysicalPath(const ST::string& path);
    void addDniFile(const ST::string& filename);
    void debug_print();

    imStream* open(const ST::string& path);

    imStream* openSdb(unsigned int node)
    {
        return open(ST::format("/sdb/{}.vdb", node));
    }

    imStream* openMdb(unsigned int node)
    {
        return open(ST::format("/mdb/{}.vdb", node));
    }

private:
    std::shared_ptr<imVfsDirEntry> m_root;
    std::map<ST::string, imStream*> m_dniStreams;
};


ST::string path_filename(const ST::string& path);
ST::string path_pathname(const ST::string& path);
bool file_exists(const ST::string& path);

std::shared_ptr<imVfsEntry> read_dnifile(imStream* stream, const ST::string& location);

#endif
