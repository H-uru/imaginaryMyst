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

imString imStream::readString(size_t length)
{
    char* buffer = new char[length];
    read(buffer, length);
    imString str(buffer, length);
    delete[] buffer;
    return str;
}

imString imStream::readZString()
{
    zstring_buffer strbuf;
    char ch;
    while (!eof() && ((ch = readByte()) != 0))
        strbuf.append(ch);
    return imString(strbuf.m_buf, strbuf.m_size);
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

imString imBufferStream::readLine()
{
    const unsigned char* lnEnd = m_ptr;
    while (lnEnd < m_end && *lnEnd != '\r' && *lnEnd != '\n')
        lnEnd++;

    imString ln((const char*)m_ptr, lnEnd - m_ptr);
    if (lnEnd[0] == '\r' && lnEnd < m_end && lnEnd[1] == '\n')
        m_ptr = lnEnd + 2;  // Windows EOL
    else
        m_ptr = lnEnd + 1;
    return ln;
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

imString imFileStream::readLine()
{
    char buf[4096];
    memset(buf, 0, 4096);
    fgets(buf, 4096, m_file);
    size_t len = strlen(buf);
    if (len > 0 && buf[len-1] == '\n')
        len--;
    if (len > 0 && buf[len-1] == '\r')
        len--;
    return imString(buf, len);
}
