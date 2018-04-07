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

#include <string_theory/string_stream>
#include <cstring>

/* imStream */
ST::string imStream::readString(size_t length)
{
    ST::char_buffer buffer;
    buffer.allocate(length);
    read(buffer.data(), length);
    return ST::string::from_latin_1(buffer);
}

ST::string imStream::readZString()
{
    ST::string_stream strbuf;
    char ch;
    while (!eof() && ((ch = readByte()) != 0))
        strbuf.append_char(ch);
    return strbuf.to_string(false);
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

ST::string imBufferStream::readLine()
{
    const unsigned char* lnEnd = m_ptr;
    while (lnEnd < m_end && *lnEnd != '\r' && *lnEnd != '\n')
        lnEnd++;

    ST::string ln = ST::string::from_latin_1((const char*)m_ptr, lnEnd - m_ptr);
    if (lnEnd[0] == '\r' && lnEnd < m_end && lnEnd[1] == '\n')
        m_ptr = lnEnd + 2;  // Windows EOL
    else
        m_ptr = lnEnd + 1;
    return ln;
}


/* imFileStream */
bool imFileStream::open(const ST::string& filename, const char* mode)
{
    close();
    m_file = fopen(filename.c_str(), mode);
    if (m_file == 0)
        return false;

    fseek(m_file, 0, SEEK_END);
    m_size = ftell(m_file);
    fseek(m_file, 0, SEEK_SET);
    return true;
}

ST::string imFileStream::readLine()
{
    char buf[4096];
    memset(buf, 0, 4096);
    fgets(buf, 4096, m_file);
    size_t len = strlen(buf);
    if (len > 0 && buf[len-1] == '\n')
        len--;
    if (len > 0 && buf[len-1] == '\r')
        len--;
    return ST::string::from_latin_1(buf, len);
}
