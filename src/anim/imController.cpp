#include "imLeafController.h"

/* imSimplePosController */
imSimplePosController::~imSimplePosController()
{
    if (m_position != 0)
        delete m_position;
}

void imSimplePosController::read(imStream* stream)
{
    if (m_position != 0)
        delete m_position;

    if (stream->read32() != 0) {
        m_position = new imPoint3Controller();
        m_position->read(stream);
    } else {
        m_position = 0;
    }
}


/* imCompoundPosController */
imCompoundPosController::~imCompoundPosController()
{
    if (m_XController != 0)
        delete m_XController;
    if (m_YController != 0)
        delete m_XController;
    if (m_ZController != 0)
        delete m_XController;
}

void imCompoundPosController::read(imStream* stream)
{
    if (m_XController != 0)
        delete m_XController;
    if (m_YController != 0)
        delete m_XController;
    if (m_ZController != 0)
        delete m_XController;

    if (stream->read32() != 0) {
        m_XController = new imScalarController();
        m_XController->read(stream);
    } else {
        m_XController = 0;
    }

    if (stream->read32() != 0) {
        m_YController = new imScalarController();
        m_YController->read(stream);
    } else {
        m_YController = 0;
    }

    if (stream->read32() != 0) {
        m_ZController = new imScalarController();
        m_ZController->read(stream);
    } else {
        m_ZController = 0;
    }
}
