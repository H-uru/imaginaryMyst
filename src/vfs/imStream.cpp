#include "imStream.h"
#include <cstring>

/* imStream */
struct zstring_buffer {
    char* m_buf;
    size_t m_alloc;
    size_t m_size;

    zstring_buffer()
    {
        m_buf = new char[256];
        m_alloc = 256;
        m_size = 0;
    }

    ~zstring_buffer()
    {
        delete[] m_buf;
    }

    void append(char ch)
    {
        if (m_size >= m_alloc) {
            // Expand the buffer
            size_t bigger = m_alloc * 2;
            char* bufTemp = new char[bigger];
            memcpy(bufTemp, m_buf, m_alloc);
            delete[] m_buf;
            m_buf = bufTemp;
            m_alloc = bigger;
        }
        m_buf[m_size++] = ch;
    }
};

imString imStream::readZString()
{
    zstring_buffer strbuf;
    char ch;
    while (!eof() && ((ch = readByte()) != 0))
        strbuf.append(ch);
    return imString(strbuf.m_buf, strbuf.m_size);
}

void imStream::writeZString(imString str)
{
    write(str.data(), str.length());
    writeByte(0);
}


/* imBufferStream */
size_t imBufferStream::read(void* buffer, size_t count)
{
    if (m_ptr + count > m_end)
        count = (size_t)(m_end - m_ptr);
    memcpy(buffer, m_ptr, count);
    m_ptr += count;
    return count;
}


/* imFileStream */
bool imFileStream::open(const char* filename, const char* mode)
{
    close();
    m_file = fopen(filename, mode);
    if (m_file == 0)
        return false;

    fseek(m_file, 0, SEEK_END);
    m_size = ftell(m_file);
    fseek(m_file, 0, SEEK_SET);
    return true;
}
