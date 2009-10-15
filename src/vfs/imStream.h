#ifndef _IM_STREAM_H
#define _IM_STREAM_H

#include "../imString.h"

#define STREAM_READ_BASIC(type, func) \
    type func() \
    { \
        type v; \
        read(&v, sizeof(v)); \
        return v; \
    }

#define STREAM_WRITE_BASIC(type, func) \
    void func(type v) \
    { \
        write(&v, sizeof(v)); \
    }

class imStream {
public:
    imStream()
    { }

    virtual ~imStream()
    { }

    STREAM_READ_BASIC(unsigned char, readByte)
    STREAM_READ_BASIC(unsigned short, read16)
    STREAM_READ_BASIC(unsigned int, read32)
    STREAM_READ_BASIC(float, readFloat)
    STREAM_READ_BASIC(double, readDouble)

    imString readString(size_t length);
    imString readZString();

    STREAM_WRITE_BASIC(unsigned char, writeByte)
    STREAM_WRITE_BASIC(unsigned short, write16)
    STREAM_WRITE_BASIC(unsigned int, write32)
    STREAM_WRITE_BASIC(float, writeFloat)
    STREAM_WRITE_BASIC(double, writeDouble)

    void writeString(imString str)
    {
        write(str.data(), str.length());
    }

    void writeZString(imString str)
    {
        write(str.data(), str.length());
        writeByte(0);
    }

    virtual size_t read(void* buffer, size_t count) = 0;
    virtual size_t write(const void* buffer, size_t count) = 0;

    virtual size_t size() const = 0;
    virtual bool eof() const = 0;
    virtual size_t tell() const = 0;
    virtual void seek(size_t pos) = 0;
};


class imBufferStream : public imStream {
public:
    imBufferStream(const unsigned char* buffer, size_t size, bool canDelete)
    : m_buffer(buffer), m_ptr(buffer), m_end(buffer + size),
      m_canDelete(canDelete)
    { }

    virtual ~imBufferStream()
    {
        if (m_canDelete)
            delete[] m_buffer;
    }

    virtual size_t read(void* buffer, size_t count);

    virtual size_t write(const void* buffer, size_t count)
    {
        // Writing to a const buffer is not supported
        return -1;
    }

    virtual size_t size() const
    { return (size_t)(m_end - m_buffer); }

    virtual bool eof() const
    { return (m_ptr >= m_end); }

    virtual size_t tell() const
    { return (size_t)(m_ptr - m_buffer); }

    virtual void seek(size_t pos)
    {
        m_ptr = m_buffer + pos;
    }

private:
    const unsigned char* m_buffer;
    const unsigned char* m_ptr;
    const unsigned char* m_end;
    bool m_canDelete;
};


class imFileStream : public imStream {
public:
    imFileStream() : m_file(0), m_size(0)
    { }

    virtual ~imFileStream()
    {
        if (m_file != 0)
            fclose(m_file);
    }

    bool open(const char* filename, const char* mode);

    void close()
    {
        if (m_file != 0)
            fclose(m_file);
        m_size = 0;
        m_file = 0;
    }

    virtual size_t read(void* buffer, size_t count)
    {
        return fread(buffer, 1, count, m_file);
    }

    virtual size_t write(const void* buffer, size_t count)
    {
        return fwrite(buffer, 1, count, m_file);
    }

    virtual size_t size() const
    { return m_size; }

    virtual bool eof() const
    {
        int ch = fgetc(m_file);
        ungetc(ch, m_file);
        return (ch == EOF);
    }

    virtual size_t tell() const
    {
        return ftell(m_file);
    }

    virtual void seek(size_t pos)
    {
        fseek(m_file, pos, SEEK_SET);
    }

private:
    FILE* m_file;
    size_t m_size;
};

/* Clean up our namespace -- because we care */
#undef STREAM_READ_BASIC
#undef STREAM_WRITE_BASIC

#endif
