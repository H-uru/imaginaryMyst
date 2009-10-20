#include "imSceneDatabase.h"
#include "../imCommon.h"

/* imObjectDatabase */
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

std::vector<unsigned int> imObjectDatabase::get(imString name) const
{
    std::map<imString, std::vector<unsigned int> >::const_iterator f =
        m_objects.find(name);
    if (f == m_objects.end())
        return std::vector<unsigned int>();
    return f->second;
}


/* imSceneDatabase */
bool imSceneDatabase::readIndex(imStream* stream)
{
    if (stream->read32BE() != 3) {
        imLog("Fatal: Invalid Scene Database!\n");
        return false;
    }

    m_saveGroups.readIndex(stream);
    m_rooms.readIndex(stream);
    m_namedGroups.readIndex(stream);
    return true;
}
