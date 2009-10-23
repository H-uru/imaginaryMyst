#include "imGeometry3.h"

/* imMatrix44 */
const imMatrix44 imMatrix44::IDENTITY(
    1.0f, 0.0f, 0.0f, 0.0f,
    0.0f, 1.0f, 0.0f, 0.0f,
    0.0f, 0.0f, 1.0f, 0.0f,
    0.0f, 0.0f, 0.0f, 1.0f );

imMatrix44::imMatrix44(float _11, float _12, float _13, float _14,
                       float _21, float _22, float _23, float _24,
                       float _31, float _32, float _33, float _34,
                       float _41, float _42, float _43, float _44)
{
    m_data[ 0] = _11;
    m_data[ 1] = _12;
    m_data[ 2] = _13;
    m_data[ 3] = _14;
    m_data[ 4] = _21;
    m_data[ 5] = _22;
    m_data[ 6] = _23;
    m_data[ 7] = _24;
    m_data[ 8] = _31;
    m_data[ 9] = _32;
    m_data[10] = _33;
    m_data[11] = _34;
    m_data[12] = _41;
    m_data[13] = _42;
    m_data[14] = _43;
    m_data[15] = _44;
}

void imMatrix44::read(imStream* stream)
{
    m_data[ 0] = stream->readFloat();
    m_data[ 1] = stream->readFloat();
    m_data[ 2] = stream->readFloat();
    m_data[ 3] = stream->readFloat();
    m_data[ 4] = stream->readFloat();
    m_data[ 5] = stream->readFloat();
    m_data[ 6] = stream->readFloat();
    m_data[ 7] = stream->readFloat();
    m_data[ 8] = stream->readFloat();
    m_data[ 9] = stream->readFloat();
    m_data[10] = stream->readFloat();
    m_data[11] = stream->readFloat();
    m_data[12] = stream->readFloat();
    m_data[13] = stream->readFloat();
    m_data[14] = stream->readFloat();
    m_data[15] = stream->readFloat();
}
