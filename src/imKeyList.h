#ifndef _IM_KEYLIST_H
#define _IM_KEYLIST_H

#include "vfs/imStream.h"
#include <vector>

class imKey {
public:
    imKey() : m_type(0xFF)
    { }

    void read(imStream* stream);

    imString name() const
    { return m_name; }

    unsigned char type() const
    { return m_type; }

    bool isValid() const
    { return (m_type != 0xFF); }

private:
    imString m_name;
    unsigned char m_type;
};

class imKeyList {
public:
    imKeyList()
    { }

    void read(imStream* stream);

    const imKey& self() const
    { return m_self; }

    const std::vector<imKey>& refs() const
    { return m_refs; }

private:
    imKey m_self;
    std::vector<imKey> m_refs;
};

#endif
