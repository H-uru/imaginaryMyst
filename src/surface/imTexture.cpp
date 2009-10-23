#include "imTexture.h"
#include "../imCommon.h"

bool imTexture::read(imStream* stream)
{
    if (stream->read32() != 0x77bd62c4) {
        imLog("Invalid Texture Magic");
        return false;
    }
    stream->read32();   // Data size

    m_keys.read(stream);
    imLog("DEBUG: [SDB] [TEXTURE] '%s' (%u)", m_keys.self().name().data(),
          m_keys.refs().size());
    m_readFlags = stream->read32();
    imLog("DEBUG: [SDB] [TEXTURE] (Read Flags) %08X", m_readFlags);

    if (stream->read32() != 0x3333) {
        imLog("Invalid hsGTexture Magic");
        return false;
    }
    size_t len = stream->read32();
    m_textureName = stream->readString(len);
    imLog("DEBUG: [SDB] [TEXTURE] (Texture Name) '%s'", m_textureName.data());
    m_textureFlags = stream->read32();
    imLog("DEBUG: [SDB] [TEXTURE] (Texture Flags) %08X", m_textureFlags);
    return true;
}
