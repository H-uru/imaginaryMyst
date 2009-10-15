#include "imSceneDatabase.h"
#include "../imCommon.h"

bool imObjectDatabase::readIndex(imStream* stream)
{
    size_t count = stream->read32();
    for (size_t i=0; i<count; i++) {
        size_t slen = stream->read32();
        imString name = stream->readString(slen);

        std::vector<unsigned int> refs;
        refs.resize(stream->read32());
        for (size_t j=0; j<refs.size(); j++)
            refs[j] = stream->read32();
        m_objects[name] = refs;
    }
    return true;
}


bool imSceneDatabase::readIndex(imStream* stream)
{
    if (stream->read32() != 0x03000000) {
        imLog("Fatal: Invalid Scene Database!\n");
        return false;
    }

    m_saveGroups.readIndex(stream);
    m_rooms.readIndex(stream);
    m_namedGroups.readIndex(stream);
    return true;
}
