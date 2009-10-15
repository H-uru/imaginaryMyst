#include "imString.h"
#include <cstring>
#include <ctype.h>

imString::imString(const char* data, size_t length)
        : m_data(0)
{
    if (data != 0) {
        if (length == (size_t)-1)
            length = strlen(data);
        m_data = new _ref(data, length);
    }
}

bool imString::operator==(const char* strData) const
{
    if (m_data == 0)
        return (strData == 0) || (strData[0] == 0);
    if (strData == 0)
        return m_data->m_length == 0;

    return strcmp(m_data->m_data, strData) == 0;
}

bool imString::operator==(const imString& ref) const
{
    if (m_data == ref.m_data)
        return true;
    if (ref.m_data == 0)
        return m_data->m_length == 0;
    if (m_data == 0)
        return ref.m_data->m_length == 0;

    return strcmp(m_data->m_data, ref.m_data->m_data) == 0;
}

imString& imString::operator=(const char* strData)
{
    if (m_data != 0)
        m_data->delRef();
    m_data = 0;

    if (strData != 0) {
        size_t length = strlen(strData);
        m_data = new _ref(strData, length);
    }
    return *this;
}

int imString::compare(const char* strData) const
{
    if (m_data == 0)
        return -(((int)strData) & 1);
    if (strData == 0)
        return 1;
    return strcmp(m_data->m_data, strData);
}

int imString::compare(const imString& ref) const
{
    if (m_data == 0)
        return -(((int)ref.m_data) & 1);
    if (ref.m_data == 0)
        return 1;
    return strcmp(m_data->m_data, ref.m_data->m_data);
}

imString imString::join(_ref* left, const char* right)
{
    size_t length = strlen(right);
    if (left == 0)
        return new _ref(right, length);

    _ref* data = new _ref();
    data->m_length = left->m_length + length;
    data->m_data = new char[data->m_length+1];
    memcpy(data->m_data, left->m_data, left->m_length);
    memcpy(data->m_data + left->m_length, right, length);
    data->m_data[data->m_length] = 0;
    return data;
}

imString imString::join(_ref* left, _ref* right)
{
    if (left == 0) {
        right->addRef();
        return right;
    }

    _ref* data = new _ref();
    data->m_length = left->m_length + right->m_length;
    data->m_data = new char[data->m_length+1];
    memcpy(data->m_data, left->m_data, left->m_length);
    memcpy(data->m_data + left->m_length, right->m_data, right->m_length);
    data->m_data[data->m_length] = 0;
    return data;
}

imString imString::mid(size_t start, size_t count) const
{
    if (m_data == 0)
        return imString();

    if (count == (size_t)-1 || (start + count) > m_data->m_length)
        count = m_data->m_length - start;
    return new _ref(m_data->m_data + start, count);
}

size_t imString::find(char ch) const
{
    if (m_data == 0)
        return (size_t)-1;

    char* ptr = m_data->m_data;
    while (*ptr != 0 && *ptr != ch)
        ++ptr;
    return (*ptr == ch) ? (ptr - m_data->m_data) : (size_t)-1;
}

size_t imString::r_find(char ch) const
{
    if (m_data == 0)
        return (size_t)-1;

    char* ptr = m_data->m_data + m_data->m_length - 1;
    while (ptr >= m_data->m_data && *ptr != ch)
        --ptr;
    return (ptr >= m_data->m_data) ? (ptr - m_data->m_data) : (size_t)-1;
}

imString imString::toLower() const
{
    if (m_data == 0)
        return imString();

    _ref* data = new _ref();
    data->m_length = m_data->m_length;
    data->m_data = new char[data->m_length+1];

    char* cp = m_data->m_data;
    char* dp = data->m_data;
    while (*cp != 0)
        *dp++ = tolower(*cp++);
    *dp = 0;
    return data;
}

imString imString::toUpper() const
{
    if (m_data == 0)
        return imString();

    _ref* data = new _ref();
    data->m_length = m_data->m_length;
    data->m_data = new char[data->m_length+1];

    char* cp = m_data->m_data;
    char* dp = data->m_data;
    while (*cp != 0)
        *dp++ = toupper(*cp++);
    *dp = 0;
    return data;
}

imString imString::Format(const char* fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    imString str = FormatV(fmt, args);
    va_end(args);
    return str;
}

imString imString::FormatV(const char* fmt, va_list args)
{
    char buf[4096];
    vsnprintf(buf, 4096, fmt, args);
    return buf;
}


/* imString _ref */
imString::_ref::_ref(const char* data, size_t length)
        : m_length(length)
{
    m_data = new char[length+1];
    memcpy(m_data, data, length);
    m_data[length] = 0;
}
