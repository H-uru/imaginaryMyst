#ifndef _IM_TEXTURE_H
#define _IM_TEXTURE_H

#include "../imKeyList.h"

class imTexture : public imDataRef {
public:
    imTexture() : m_readFlags(0), m_textureFlags(0)
    { }

    ~imTexture()
    { }

    bool read(imStream* stream);

    const imKeyList& keys() const
    { return m_keys; }

    imString textureName() const
    { return m_textureName; }

    unsigned int flags() const
    { return m_textureFlags; }

private:
    imKeyList m_keys;
    unsigned int m_readFlags;
    imString m_textureName;
    unsigned int m_textureFlags;
};

#endif
