#ifndef _IM_ENVIRONMENT_H
#define _IM_ENVIRONMENT_H

#include "../surface/imMaterial.h"

class imEnvironment {
public:
    enum EnvironFlags {
        kHas_2 = (1<<1),
        kHas_4 = (1<<2),
        kHasFogDepth = (1<<3),
        kHasFogColor = (1<<4),
        kHasFogDensity = (1<<5),
        kHasYon = (1<<6),
        kHas_200 = (1<<9),
        kHasClearColor = (1<<11),
        kHasFogControl = (1<<13),
        kHasRenderProcs = (1<<15),
    };

    imEnvironment()
    { }

    ~imEnvironment()
    { }

    bool read(imStream* stream, class imSceneIndex* idx);

private:
    imKeyList m_keys;
    unsigned int m_environFlags;
    float m_fogDensity, m_fogDepth, m_yon;
    imColorRGBA m_fogColor, m_clearColor;
    imRef<imMaterial> m_material;
    std::vector<imKey> m_refs;
};

#endif
