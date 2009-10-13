#ifndef _IM_STRING_H
#define _IM_STRING_H

#include "imCommon.h"

class imString {
public:
    inline imString();
    imString(const char* strData, size_t length = (size_t)-1);
    inline imString(const imString& ref);
    inline ~imString();

    inline operator const char*() const;
    bool operator==(const char* strData) const;
    bool operator==(const imString& ref) const;
    imString& operator=(const char* strData);
    inline imString& operator=(const imString& ref);
    inline bool operator<(const imString& ref) const;

    inline bool isNull() const;
    inline bool isEmpty() const;
    inline const char* data() const;
    inline size_t length() const;

    int compare(const char* strData) const;
    int compare(const imString& ref) const;

    inline imString operator+(const char* str) const;
    inline imString operator+(const imString& ref) const;

    imString mid(size_t start, size_t count = (size_t)-1) const;
    inline imString left(size_t count) const;
    inline imString right(size_t count) const;
    size_t find(char ch) const;
    size_t r_find(char ch) const;

    imString toLower() const;
    imString toUpper() const;

private:
    class _ref : public imDataRef {
    public:
        char* m_data;
        size_t m_length;

        _ref(const char* data, size_t length);
        inline _ref();
        inline ~_ref();
    } *m_data;

    inline imString(_ref* data);

    static imString join(_ref* left, const char* right);
    static imString join(_ref* left, _ref* right);
};


/* imString inlines */
imString::imString()
        : m_data(0)
{ }

imString::imString(const imString& ref)
        : m_data(ref.m_data)
{
    if (m_data != 0)
        m_data->addRef();
}

imString::imString(_ref* data)
        : m_data(data)
{ }

imString::~imString()
{
    if (m_data != 0)
        m_data->delRef();
}

imString::operator const char*() const
{ return (m_data != 0) ? m_data->m_data : 0; }

imString& imString::operator=(const imString& ref)
{
    if (ref.m_data != 0)
        ref.m_data->addRef();
    if (m_data != 0)
        m_data->delRef();
    m_data = ref.m_data;
    return *this;
}

bool imString::operator<(const imString& ref) const
{ return compare(ref) < 0; }

bool imString::isNull() const
{ return m_data == 0; }

bool imString::isEmpty() const
{ return isNull() || m_data->m_length == 0; }

const char* imString::data() const
{ return (m_data != 0) ? m_data->m_data : 0; }

size_t imString::length() const
{ return (m_data != 0) ? m_data->m_length : 0; }

imString imString::operator+(const char* str) const
{
    if (str == 0)
        return *this;
    return join(m_data, str);
}

imString imString::operator+(const imString& ref) const
{
    if (ref.m_data == 0)
        return *this;
    return join(m_data, ref.m_data);
}

imString imString::left(size_t count) const
{
    return mid(0, count);
}

imString imString::right(size_t count) const
{
    if (m_data == 0)
        return imString();
    return mid(m_data->m_length - count, count);
}


/* imString _ref inlines */
imString::_ref::_ref()
{ }

imString::_ref::~_ref()
{ delete[] m_data; }

#endif
