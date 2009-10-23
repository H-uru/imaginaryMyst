#include "imSceneIndex.h"
#include "../imCommon.h"

bool imSceneIndex::read(imStream* stream)
{
    if (stream->read32BE() != 2) {
        imLog("Invalid header version");
        return false;
    }

    size_t len;
    imLog("DEBUG: [SDB] (Header +4) %d", stream->read32());
    stream->read32();   // File size
    len = stream->read32();
    m_name = stream->readString(len);
    len = stream->read32();
    m_page = stream->readString(len);
    imLog("DEBUG: [SDB] Name: '%s', '%s'", m_name.data(), m_page.data());
    m_sceneRefs.resize(stream->read32());
    for (size_t i=0; i<m_sceneRefs.size(); i++) {
        len = stream->read32();
        m_sceneRefs[i] = stream->readString(len);
        imLog("DEBUG: [SDB] * SceneRef: '%s'", m_sceneRefs[i].data());
    }

    if (stream->read32BE() != 0x4edf6994) {
        imLog("Invalid Database Magic");
        return false;
    }
    unsigned int verMaj = stream->read32BE();
    if (verMaj != 2) {
        imLog("Unsupported Database Major Version %d", verMaj);
        return false;
    }
    unsigned int verMin = stream->read32();
    if (verMin != 6) {
        imLog("Unsupported Database Minor Version %d", verMin);
        return false;
    }

    len = stream->read32();
    imLog("DEBUG: [SDB] (+0C) %s", stream->readString(len).data());
    imLog("DEBUG: [SDB] Key Refs:");
    m_keyRefs.resize(stream->read32());
    for (size_t i=0; i<m_keyRefs.size(); i++) {
        len = stream->read32();
        m_keyRefs[i] = stream->readString(len);
        imLog("DEBUG: [SDB] * '%s'", m_keyRefs[i].data());
    }

    m_textures.resize(stream->read32());
    for (size_t i=0; i<m_textures.size(); i++) {
        imLog("DEBUG: [SDB] Texture #%u", i);
        m_textures[i] = new imTexture();
        if (!m_textures[i]->read(stream))
            return false;
    }

    m_materials.resize(stream->read32());
    for (size_t i=0; i<m_materials.size(); i++) {
        imLog("DEBUG: [SDB] Material #%u", i);
        m_materials[i] = new imMaterial();
        if (!m_materials[i]->read(stream, this))
            return false;
    }

    return true;
}
