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
    inline imVfsEntry(imString name, VfsType type);
    inline virtual ~imVfsEntry();

    inline bool isDirectory() const;
    inline bool isCompressed() const;
    inline imString name() const;
    inline VfsType type() const;

    inline void setCompressed();

private:
    imString m_name;
    VfsType m_type;
};


class imVfsDirEntry : public imVfsEntry {
public:
    inline imVfsDirEntry();
    inline imVfsDirEntry(imString name, const std::list<imRef<imVfsEntry> >& children);
    inline virtual ~imVfsDirEntry();

    std::list<imRef<imVfsEntry> > children() const;
    inline imRef<imVfsEntry> get(imString name) const;

    void add(const std::list<imRef<imVfsEntry> >& children);

private:
    // Map these by the entry name for faster lookups
    std::map<imString, imRef<imVfsEntry> > m_children;
};


class imVfsFileEntry : public imVfsEntry {
public:
    inline imVfsFileEntry(imString name, VfsType type,
                          size_t size, imString location);
    inline imVfsFileEntry(imString name, VfsType type,
                          unsigned int vext, size_t offset,
                          size_t size, size_t csize, imString location);
    inline virtual ~imVfsFileEntry();

    inline unsigned int vext() const;
    inline size_t voffset() const;
    inline size_t size() const;
    inline size_t csize() const;
    inline imString location() const;

private:
    unsigned int m_vext;    // Still don't know what this is...
    size_t m_voffset;       // Offset in Virtual files
    size_t m_size;          // Size of file
    size_t m_csize;         // Compressed (Zlib) size
    imString m_location;    // Where the file is stored
};


class imVfs {
public:
    inline imVfs();
    ~imVfs();

    void addPhysicalPath(imString path);
    void addDniFile(imString filename);
    void debug_print();

    imStream* open(imString path);

private:
    imRef<imVfsDirEntry> m_root;
    std::map<imString, imStream*> m_dniStreams;
};


/* imVfsEntry inlines */
imVfsEntry::imVfsEntry(imString name, VfsType type)
          : m_name(name), m_type(type)
{ }

imVfsEntry::~imVfsEntry()
{ }

bool imVfsEntry::isDirectory() const
{
    return ((m_type & Type_Mask) == Type_Directory);
}

bool imVfsEntry::isCompressed() const
{ return (m_type & Type_ZLib) != 0; }

imString imVfsEntry::name() const
{ return m_name; }

imVfsEntry::VfsType imVfsEntry::type() const
{ return (VfsType)(m_type & Type_Mask); }

void imVfsEntry::setCompressed()
{ m_type = (VfsType)(m_type | Type_ZLib); }


/* imVfsDirEntry inlines */
imVfsDirEntry::imVfsDirEntry()
             : imVfsEntry(imString(), Type_Directory)
{ }

imVfsDirEntry::imVfsDirEntry(imString name,
                             const std::list<imRef<imVfsEntry> >& children)
             : imVfsEntry(name, Type_Directory)
{
    add(children);
}

imVfsDirEntry::~imVfsDirEntry()
{ }

imRef<imVfsEntry> imVfsDirEntry::get(imString name) const
{
    name = name.toLower();
    std::map<imString, imRef<imVfsEntry> >::const_iterator f = m_children.find(name);
    if (f == m_children.end())
        return 0;
    return f->second;
}


/* imVfsFileEntry inlines */
imVfsFileEntry::imVfsFileEntry(imString name, VfsType type,
                               size_t size, imString location)
              : imVfsEntry(name, type), m_vext(0), m_voffset(0), m_size(size),
                m_csize(0), m_location(location)
{ }

imVfsFileEntry::imVfsFileEntry(imString name, VfsType type,
                               unsigned int vext, size_t offset,
                               size_t size, size_t csize, imString location)
              : imVfsEntry(name, type), m_vext(vext), m_voffset(offset),
                m_size(size), m_csize(csize), m_location(location)
{
    if (m_csize != 0)
        setCompressed();
}

imVfsFileEntry::~imVfsFileEntry()
{ }

unsigned int imVfsFileEntry::vext() const
{ return m_vext; }

unsigned int imVfsFileEntry::voffset() const
{ return m_voffset; }

size_t imVfsFileEntry::size() const
{ return m_size; }

size_t imVfsFileEntry::csize() const
{ return m_csize; }

imString imVfsFileEntry::location() const
{ return m_location; }


/* imVfs inlines */
imVfs::imVfs()
{
    m_root = new imVfsDirEntry();
}


imString path_filename(imString path);
imVfsEntry* read_dnifile(imStream* stream, imString location);

#endif
