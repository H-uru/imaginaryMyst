#ifndef _IM_SCENEDATABASE_H
#define _IM_SCENEDATABASE_H

#include <map>
#include <vector>
#include "../vfs/imStream.h"

class imObjectDatabase {
public:
    imObjectDatabase()
    { }

    ~imObjectDatabase()
    { }

    bool readIndex(imStream* stream);
    std::vector<unsigned int> get(imString name) const;

private:
    std::map<imString, std::vector<unsigned int> > m_objects;
};

class imSceneDatabase {
public:
    imSceneDatabase()
    { }

    ~imSceneDatabase()
    { }

    bool readIndex(imStream* stream);

    std::vector<unsigned int> saveGroup(imString name)
    { return m_saveGroups.get(name); }

    std::vector<unsigned int> room(imString name)
    { return m_rooms.get(name); }

    std::vector<unsigned int> namedGroup(imString name)
    { return m_namedGroups.get(name); }

private:
    imObjectDatabase m_saveGroups;
    imObjectDatabase m_rooms;
    imObjectDatabase m_namedGroups;
};

#endif
