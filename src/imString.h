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

#ifndef _IM_STRING_H
#define _IM_STRING_H

#include <cstdarg>
#include <cstdlib>
#include <cstdio>
#include "imRef.h"

class imString {
public:
    imString() : m_data(0)
    { }

    imString(const char* strData, size_t length = (size_t)-1);

    imString(const imString& ref) : m_data(ref.m_data)
    {
        if (m_data != 0)
            m_data->addRef();
    }

    ~imString()
    {
        if (m_data != 0)
            m_data->delRef();
    }

    operator const char*() const
    { return (m_data != 0) ? m_data->m_data : 0; }

    bool operator==(const char* strData) const;
    bool operator==(const imString& ref) const;
    imString& operator=(const char* strData);

    imString& operator=(const imString& ref)
    {
        if (ref.m_data != 0)
            ref.m_data->addRef();
        if (m_data != 0)
            m_data->delRef();
        m_data = ref.m_data;
        return *this;
    }

    bool operator<(const imString& ref) const
    { return compare(ref) < 0; }

    bool isNull() const
    { return m_data == 0; }

    bool isEmpty() const
    { return isNull() || m_data->m_length == 0; }

    const char* data() const
    { return (m_data != 0) ? m_data->m_data : 0; }

    size_t length() const
    { return (m_data != 0) ? m_data->m_length : 0; }

    int compare(const char* strData) const;
    int compare(const imString& ref) const;

    imString operator+(const char* str) const
    {
        if (str == 0)
            return *this;
        return join(m_data, str);
    }

    imString operator+(const imString& ref) const
    {
        if (ref.m_data == 0)
            return *this;
        return join(m_data, ref.m_data);
    }

    imString mid(size_t start, size_t count = (size_t)-1) const;

    imString left(size_t count) const
    {
        return mid(0, count);
    }

    imString right(size_t count) const
    {
        if (m_data == 0)
            return imString();
        return mid(m_data->m_length - count, count);
    }

    size_t find(char ch) const;
    size_t r_find(char ch) const;

    imString toLower() const;
    imString toUpper() const;
    imString strip() const;

    static imString Format(const char* fmt, ...);
    static imString FormatV(const char* fmt, va_list args);

private:
    class _ref : public imDataRef {
    public:
        _ref(const char* data, size_t length);

        _ref()
        { }

        ~_ref()
        { delete[] m_data; }

    public:
        char* m_data;
        size_t m_length;
    } *m_data;

    imString(_ref* data) : m_data(data)
    { }

    static imString join(_ref* left, const char* right);
    static imString join(_ref* left, _ref* right);
};

#endif
