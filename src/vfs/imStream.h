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

#ifndef _IM_STREAM_H
#define _IM_STREAM_H

#include <string_theory/string>

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

    unsigned int read32BE()
    {
        unsigned int v = read32();
        return ((v & 0x000000FF) << 24) | ((v & 0x0000FF00) << 8)
             | ((v & 0xFF000000) >> 24) | ((v & 0x00FF0000) >> 8);
    }

    ST::string readString(size_t length);
    ST::string readZString();

    STREAM_WRITE_BASIC(unsigned char, writeByte)
    STREAM_WRITE_BASIC(unsigned short, write16)
    STREAM_WRITE_BASIC(unsigned int, write32)
    STREAM_WRITE_BASIC(float, writeFloat)
    STREAM_WRITE_BASIC(double, writeDouble)

    void write32BE(unsigned int v)
    {
        v = ((v & 0x000000FF) << 24) | ((v & 0x0000FF00) << 8)
          | ((v & 0xFF000000) >> 24) | ((v & 0x00FF0000) >> 8);
        write32(v);
    }

    void writeString(const ST::string& str)
    {
        write(str.c_str(), str.size());
    }

    void writeZString(const ST::string& str)
    {
        write(str.c_str(), str.size());
        writeByte(0);
    }

    virtual size_t read(void* buffer, size_t count) = 0;
    virtual size_t write(const void* buffer, size_t count) = 0;
    virtual ST::string readLine() = 0;

    virtual size_t size() const = 0;
    virtual bool eof() const = 0;
    virtual size_t tell() const = 0;
    virtual void seek(size_t pos) = 0;
};


class imBufferStream : public imStream {
public:
    imBufferStream(const unsigned char* buffer, size_t size, bool owned)
        : m_buffer(buffer), m_ptr(buffer), m_end(buffer + size),
          m_owned(owned)
    { }

    virtual ~imBufferStream()
    {
        if (m_owned)
            delete[] m_buffer;
    }

    virtual size_t read(void* buffer, size_t count);

    virtual size_t write(const void* buffer, size_t count)
    {
        // Writing to a const buffer is not supported
        return -1;
    }

    virtual ST::string readLine();

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
    bool m_owned;
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

    bool open(const ST::string& filename, const char* mode);

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

    virtual ST::string readLine();

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
