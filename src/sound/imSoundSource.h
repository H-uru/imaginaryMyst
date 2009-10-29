#ifndef _IM_SOUNDSOURCE_H
#define _IM_SOUNDSOURCE_H

#include "../imKeyList.h"
#include "../imRef.h"

class imSoundSource : public imDataRef {
public:
    imSoundSource()
    { }

    ~imSoundSource()
    { }

    bool read(imStream* stream);

private:
    imKeyList m_keys;
    std::vector<imString> m_soundKeys;
    imString m_sceneObjectKey, m_animPathKey, m_listenerKey, m_observerKey;
};

#endif
