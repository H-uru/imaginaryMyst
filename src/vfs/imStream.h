#ifndef _IM_STREAM_H
#define _IM_STREAM_H

#include "../imString.h"
#include <cstdio>

class imStream {
public:
    inline imStream();
    inline virtual ~imStream();

    inline unsigned char readByte();
    inline unsigned short read16();
    inline unsigned int read32();
    inline float readFloat();
    inline double readDouble();
    imString readZString();

    inline void writeByte(unsigned char);
    inline void write16(unsigned short);
    inline void write32(unsigned int);
    inline void writeFloat(float);
    inline void writeDouble(double);
    void writeZString(imString str);

    virtual size_t read(void* buffer, size_t count) = 0;
    virtual size_t write(const void* buffer, size_t count) = 0;

    virtual size_t size() const = 0;
    virtual bool eof() const = 0;
    virtual size_t tell() const = 0;
    virtual void seek(size_t pos) = 0;
};


class imBufferStream : public imStream {
public:
    inline imBufferStream(const unsigned char* buffer, size_t size, bool canDelete);
    inline virtual ~imBufferStream();

    virtual size_t read(void* buffer, size_t count);
    inline virtual size_t write(const void* buffer, size_t count);

    inline virtual size_t size() const;
    inline virtual bool eof() const;
    inline virtual size_t tell() const;
    inline virtual void seek(size_t pos);

private:
    const unsigned char* m_buffer;
    const unsigned char* m_ptr;
    const unsigned char* m_end;
    bool m_canDelete;
};


class imFileStream : public imStream {
public:
    inline imFileStream();
    inline virtual ~imFileStream();

    bool open(const char* filename, const char* mode);
    inline void close();

    inline virtual size_t read(void* buffer, size_t count);
    inline virtual size_t write(const void* buffer, size_t count);

    inline virtual size_t size() const;
    inline virtual bool eof() const;
    inline virtual size_t tell() const;
    inline virtual void seek(size_t pos);

private:
    FILE* m_file;
    size_t m_size;
};


/* imStream inlines */
imStream::imStream()
{ }

imStream::~imStream()
{ }

#define STREAM_READ_BASIC(type, func) \
    type imStream::func() \
    { \
        type v; \
        read(&v, sizeof(v)); \
        return v; \
    }

STREAM_READ_BASIC(unsigned char, readByte)
STREAM_READ_BASIC(unsigned short, read16)
STREAM_READ_BASIC(unsigned int, read32)
STREAM_READ_BASIC(float, readFloat)
STREAM_READ_BASIC(double, readDouble)

#define STREAM_WRITE_BASIC(type, func) \
    void imStream::func(type v) \
    { \
        write(&v, sizeof(v)); \
    }

STREAM_WRITE_BASIC(unsigned char, writeByte)
STREAM_WRITE_BASIC(unsigned short, write16)
STREAM_WRITE_BASIC(unsigned int, write32)
STREAM_WRITE_BASIC(float, writeFloat)
STREAM_WRITE_BASIC(double, writeDouble)

// Clean up our namespace -- because we care
#undef STREAM_READ_BASIC
#undef STREAM_WRITE_BASIC


/* imBufferStream inlines */
imBufferStream::imBufferStream(const unsigned char* buffer, size_t size, bool canDelete)
              : m_buffer(buffer), m_ptr(buffer), m_end(buffer + size),
                m_canDelete(canDelete)
{ }

imBufferStream::~imBufferStream()
{
    if (m_canDelete)
        delete[] m_buffer;
}

size_t imBufferStream::write(const void* buffer, size_t count)
{
    // Writing to a const buffer is not supported
    return -1;
}

size_t imBufferStream::size() const
{ return (size_t)(m_end - m_buffer); }

bool imBufferStream::eof() const
{ return (m_ptr >= m_end); }

size_t imBufferStream::tell() const
{ return (size_t)(m_ptr - m_buffer); }

void imBufferStream::seek(size_t pos)
{
    m_ptr = m_buffer + pos;
}


/* imFileStream inlines */
imFileStream::imFileStream()
            : m_file(0), m_size(0)
{ }

imFileStream::~imFileStream()
{
    if (m_file != 0)
        fclose(m_file);
}

void imFileStream::close()
{
    if (m_file != 0)
        fclose(m_file);
    m_size = 0;
    m_file = 0;
}

size_t imFileStream::read(void* buffer, size_t count)
{
    return fread(buffer, 1, count, m_file);
}

size_t imFileStream::write(const void* buffer, size_t count)
{
    return fwrite(buffer, 1, count, m_file);
}

size_t imFileStream::size() const
{ return m_size; }

bool imFileStream::eof() const
{
    int ch = fgetc(m_file);
    ungetc(ch, m_file);
    return (ch == EOF);
}

size_t imFileStream::tell() const
{
    return ftell(m_file);
}

void imFileStream::seek(size_t pos)
{
    fseek(m_file, pos, SEEK_SET);
}

#endif
