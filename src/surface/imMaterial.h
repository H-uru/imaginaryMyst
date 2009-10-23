#ifndef _IM_MATERIAL_H
#define _IM_MATERIAL_H

#include <vector>
#include "imLayer.h"
#include "../imKeyList.h"

class imMaterial : public imDataRef {
public:
    imMaterial() : m_readFlags(0)
    { }

    ~imMaterial();

    bool read(imStream* stream, class imSceneIndex* idx);

    const imKeyList& keys() const
    { return m_keys; }

    const std::vector<imLayer*> layers() const
    { return m_layers; }

private:
    imKeyList m_keys;
    unsigned int m_readFlags;
    std::vector<imLayer*> m_layers;
};

#endif
