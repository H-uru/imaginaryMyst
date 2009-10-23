#include "imLeafController.h"
#include "../imCommon.h"

/* imLeafController */
imLeafController::~imLeafController()
{
    for (size_t i=0; i<m_ease.size(); i++)
        delete m_ease[i];
}

void imLeafController::read(imStream* stream)
{
    imLog("DEBUG: [SDB] [plLeafController] (UruUnknown) %d", stream->read32());

    for (size_t i=0; i<m_ease.size(); i++)
        delete m_ease[i];
    m_ease.resize(stream->read32());
    for (size_t i=0; i<m_ease.size(); i++) {
        m_ease[i] = new imEaseController();
        m_ease[i]->read(stream);
    }

    stream->read32();   // Yep, another zero
}


/* imEaseController */
void imEaseController::read(imStream* stream)
{
    imLeafController::read(stream);

    m_enabled = (stream->read32() != 0);
    if (m_enabled) {
        m_keys.resize(stream->read32());
        for (size_t i=0; i<m_keys.size(); i++)
            m_keys[i].read(stream);
    }
}


/* imScalarController */
void imScalarController::read(imStream* stream)
{
    imLeafController::read(stream);

    m_enabled = (stream->read32() != 0);
    if (m_enabled) {
        m_keys.resize(stream->read32());
        for (size_t i=0; i<m_keys.size(); i++)
            m_keys[i].read(stream);
    }
}


/* imPoint3Controller */
void imPoint3Controller::read(imStream* stream)
{
    imLeafController::read(stream);

    m_enabled = (stream->read32() != 0);
    if (m_enabled) {
        m_keys.resize(stream->read32());
        for (size_t i=0; i<m_keys.size(); i++)
            m_keys[i].read(stream);
    }
}


/* imMatrix44Controller */
void imMatrix44Controller::read(imStream* stream)
{
    imLeafController::read(stream);

    m_keys.resize(stream->read32());
    for (size_t i=0; i<m_keys.size(); i++)
        m_keys[i].read(stream);
}
