#ifndef _IM_VFS_H
#define _IM_VFS_H

#include "../imString.h"
#include "imStream.h"
#include <map>
#include <list>

class imVfsEntry : public imDataRef {
public:
    enum VfsType {
        Type_Directory, Type_File, Type_VFile,
        Type_Mask = 0x000F,

        Type_ZLib = 0x0010,
    };

public:
    imVfsEntry(imString name, VfsType type)
    : m_name(name), m_type(type)
    { }

    virtual ~imVfsEntry()
    { }

    bool isDirectory() const
    {
        return ((m_type & Type_Mask) == Type_Directory);
    }

    bool isCompressed() const
    { return (m_type & Type_ZLib) != 0; }

    imString name() const
    { return m_name; }

    VfsType type() const
    { return (VfsType)(m_type & Type_Mask); }

    void setCompressed()
    { m_type = (VfsType)(m_type | Type_ZLib); }

private:
    imString m_name;
    VfsType m_type;
};


class imVfsDirEntry : public imVfsEntry {
public:
    imVfsDirEntry()
    : imVfsEntry(imString(), Type_Directory)
    { }

    imVfsDirEntry(imString name, const std::list<imRef<imVfsEntry> >& children)
    : imVfsEntry(name, Type_Directory)
    {
        add(children);
    }

    virtual ~imVfsDirEntry()
    { }

    std::list<imRef<imVfsEntry> > children() const;
    imRef<imVfsEntry> get(imString name) const;

    void add(const std::list<imRef<imVfsEntry> >& children);

private:
    // Map these by the entry name for faster lookups
    std::map<imString, imRef<imVfsEntry> > m_children;
};


class imVfsFileEntry : public imVfsEntry {
public:
    imVfsFileEntry(imString name, VfsType type, size_t size, imString location)
    : imVfsEntry(name, type), m_vext(0), m_voffset(0), m_size(size),
      m_csize(0), m_location(location)
    { }

    imVfsFileEntry(imString name, VfsType type, unsigned int vext,
                   size_t offset, size_t size, size_t csize, imString location)
    : imVfsEntry(name, type), m_vext(vext), m_voffset(offset),
      m_size(size), m_csize(csize), m_location(location)
    {
        if (m_csize != 0)
            setCompressed();
    }

    virtual ~imVfsFileEntry()
    { }

    unsigned int vext() const
    { return m_vext; }

    size_t voffset() const
    { return m_voffset; }

    size_t size() const
    { return m_size; }

    size_t csize() const
    { return m_csize; }

    imString location() const
    { return m_location; }

private:
    unsigned int m_vext;    // Still don't know what this is...
    size_t m_voffset;       // Offset in Virtual files
    size_t m_size;          // Size of file
    size_t m_csize;         // Compressed (Zlib) size
    imString m_location;    // Where the file is stored
};


class imVfs {
public:
    imVfs()
    {
        m_root = new imVfsDirEntry();
    }

    ~imVfs();

    void addPhysicalPath(imString path);
    void addDniFile(imString filename);
    void debug_print();

    imStream* open(imString path);

private:
    imRef<imVfsDirEntry> m_root;
    std::map<imString, imStream*> m_dniStreams;
};


imString path_filename(imString path);
imVfsEntry* read_dnifile(imStream* stream, imString location);

#endif
