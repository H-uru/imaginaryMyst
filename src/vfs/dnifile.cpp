#include "imVfs.h"

#define DIRT_MAGIC      0x74726944
#define DIRT_VERSION    0x00010000

struct dni_Context {
    unsigned int m_dirOffset;
    unsigned int m_fileOffset;
    unsigned int m_namesOffset;
    unsigned int m_dataOffset;
    unsigned int m_whatever;
    imStream* m_stream;
    imString m_location;
};

static imString read_zstring(imStream* stream)
{
    unsigned int offset = stream->read32();
    size_t psave = stream->tell();
    stream->seek(offset);
    imString str = stream->readZString();
    stream->seek(psave);
    return str;
}

static imVfsEntry* read_dnidirs(dni_Context* dni, unsigned int offset)
{
    dni->m_stream->seek(offset);
    imString name = read_zstring(dni->m_stream);
    if (offset >= dni->m_fileOffset) {
        // File entry
        unsigned int vext = dni->m_stream->read32();
        size_t size = dni->m_stream->read32();
        size_t voffset = dni->m_stream->read32();
        size_t csize = dni->m_stream->read32();
        return new imVfsFileEntry(name, imVfsEntry::Type_VFile, vext, voffset,
                                  size, csize, dni->m_location);
    } else {
        size_t count = dni->m_stream->read32();
        std::list<imRef<imVfsEntry> > children;
        for (size_t i=0; i<count; i++) {
            size_t dirOff = dni->m_stream->read32();
            size_t psave = dni->m_stream->tell();
            children.push_back(read_dnidirs(dni, dirOff));
            dni->m_stream->seek(psave);
        }
        return new imVfsDirEntry(name, children);
    }
}

imVfsEntry* read_dnifile(imStream* stream, imString location)
{
    if (stream->read32() != DIRT_MAGIC)
        return 0;
    if (stream->read32() != DIRT_VERSION)
        return 0;

    dni_Context dni;
    dni.m_dirOffset = stream->read32();
    dni.m_fileOffset = stream->read32();
    dni.m_namesOffset = stream->read32();
    dni.m_dataOffset = stream->read32();
    dni.m_whatever = stream->read32();
    dni.m_stream = stream;
    dni.m_location = location;
    return read_dnidirs(&dni, dni.m_dirOffset);
}
