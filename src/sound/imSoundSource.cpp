#include "imSoundSource.h"
#include "../imCommon.h"

bool imSoundSource::read(imStream* stream)
{
    if (stream->read32() != 0x656d2f93) {
        imLog("Invalid Sound Source Magic");
        return false;
    }
    stream->read32();   // Data size

    m_keys.read(stream);
    imLog("DEBUG: [SDB] [SOUNDSOURCE] '%s' (%u)", m_keys.self().name().data(),
          m_keys.refs().size());

    imLog("DEBUG: [SDB] [SOUNDSOURCE] (+1C) %d", stream->read32());
    m_soundKeys.resize(stream->read32());
    for (size_t i=0; i<m_soundKeys.size(); i++) {
        size_t len = stream->read32();
        m_soundKeys[i] = stream->readString(len);
    }

    imLog("DEBUG: [SDB] [SOUNDSOURCE] (+38) %f", stream->readFloat());
    imLog("DEBUG: [SDB] [SOUNDSOURCE] (?) %d", stream->read32());
    imLog("DEBUG: [SDB] [SOUNDSOURCE] (+70) %f", stream->readFloat());

    size_t len = stream->read32();
    m_sceneObjectKey = stream->readString(len);
    len = stream->read32();
    m_animPathKey = stream->readString(len);
    len = stream->read32();
    m_listenerKey = stream->readString(len);
    len = stream->read32();
    m_observerKey = stream->readString(len);

    imLog("DEBUG: [SDB] [SOUNDSOURCE] (+8C) %f", stream->readFloat());
    size_t count = stream->read32();
    for (size_t i=0; i<count; i++) {
        len = stream->read32();
        imLog("DEBUG: [SDB] [SOUNDSOURCE] (+7C) '%s'",
              stream->readString(len).data());
    }

    imLog("DEBUG: [SDB] [SOUNDSOURCE] (+A4) %f", stream->readFloat());
    imLog("DEBUG: [SDB] [SOUNDSOURCE] (+A8) %f", stream->readFloat());
    imLog("DEBUG: [SDB] [SOUNDSOURCE] (+AC) %f", stream->readFloat());
    imLog("DEBUG: [SDB] [SOUNDSOURCE] (+B0) %f", stream->readFloat());
    imLog("DEBUG: [SDB] [SOUNDSOURCE] (+C0) %f", stream->readFloat());
    imLog("DEBUG: [SDB] [SOUNDSOURCE] (+90) %f", stream->readFloat());

    return true;
}
