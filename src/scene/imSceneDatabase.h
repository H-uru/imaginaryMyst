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

private:
    imObjectDatabase m_saveGroups;
    imObjectDatabase m_rooms;
    imObjectDatabase m_namedGroups;
};

#endif
