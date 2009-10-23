#ifndef _IM_LAYER_H
#define _IM_LAYER_H

#include "imTexture.h"
#include "../imGeometry3.h"
#include "../anim/imAnimBehavior.h"

struct imColorRGBA {
    imColorRGBA() : m_red(0.0f), m_green(0.0f), m_blue(0.0f), m_alpha(0.0f)
    { }

    imColorRGBA(float red, float green, float blue, float alpha = 1.0f)
    : m_red(red), m_green(green), m_blue(blue), m_alpha(alpha)
    { }

    float m_red, m_green, m_blue, m_alpha;

    void read(imStream* stream);
};

class imLayer {
public:
    enum BlendFlags {
        kBlendText = (1<<0),
        kBlendAlpha = (1<<1),
        kBlendMult = (1<<2),
        kBlendAdd = (1<<3),
        kBlendAddColorTimesAlpha = (1<<4),
        kBlendAntiAlias = (1<<5),
        kBlendDetail = (1<<6),
        kBlendNoColor = (1<<7),
        kBlendMADD = (1<<8),
        kBlendDot3 = (1<<9),
        kBlendAddSigned = (1<<10),
        kBlendAddSigned2X = (1<<11),
        kBlendInvertAlpha = (1<<12),
        kBlendInvertColor = (1<<13),
        kBlendAlphaMult = (1<<14),
        kBlendAlphaAdd = (1<<15),
        kBlendNoVtxAlpha = (1<<16),
        kBlendNoTexColor = (1<<17),
        kBlendNoTexAlpha = (1<<18),
        kBlendInvertVtxAlpha = (1<<19),
        kBlendAlphaAlways = (1<<20),
        kBlendInvertFinalColor = (1<<21),
        kBlendInvertFinalAlpha = (1<<22),
        kBlendEnvBumpNext = (1<<23),
        kBlendSubtract = (1<<24),
        kBlendRevSubtract = (1<<25),
        kBlendAlphaTestHigh = (1<<26),
    };

    enum ClampFlags {
        kClampU = (1<<0),
        kClampV = (1<<1),
    };

    enum ShadeFlags {
        kShadeSoftShadow = (1<<0),
        kShadeNoProjectors = (1<<1),
        kShadeEnvironMap = (1<<2),
        kShadeVertexShade = (1<<5),
        kShadeBlack = (1<<6),
        kShadeSpecular = (1<<7),
        kShadeNoFog = (1<<8),
        kShadeWhite = (1<<9),
        kShadeSpecularAlpha = (1<<10),
        kShadeSpecularColor = (1<<11),
        kShadeSpecularHighlight = (1<<12),
        kShadeVertColShade = (1<<13),
        kShadeInherit = (1<<14),
        kShadeIgnoreVtxIllum = (1<<15),
        kShadeEmissive = (1<<16),
        kShadeReallyNoFog = (1<<17),
    };

    enum ZFlags {
        kZIncLayer = (1<<0),
        kZClearZ = (1<<2),
        kZNoZRead = (1<<3),
        kZNoZWrite = (1<<4),
        kZLODBias = (1<<5),
    };

    enum MiscFlags {
        kMiscWireFrame = (1<<0),
        kMiscDrawMeshOutlines = (1<<1),
        kMiscTwoSided = (1<<2),
        kMiscDrawAsSplats = (1<<3),
        kMiscAdjustPlane = (1<<4),
        kMiscAdjustCylinder = (1<<5),
        kMiscAdjustSphere = (1<<6),
        kMiscTroubledLoner = (1<<7),
        kMiscBindSkip = (1<<8),
        kMiscBindMask = (1<<9),
        kMiscBindNext = (1<<10),
        kMiscLightMap = (1<<11),
        kMiscUseReflectionXform = (1<<12),
        kMiscPerspProjection = (1<<13),
        kMiscOrthoProjection = (1<<14),
        kMiscRestartPassHere = (1<<15),
        kMiscBumpLayer = (1<<16),
        kMiscBumpDu = (1<<17),
        kMiscBumpDv = (1<<18),
        kMiscBumpDw = (1<<19),
        kMiscNoShadowAlpha = (1<<20),
        kMiscUseRefractionXform = (1<<21),
        kMiscCam2Screen = (1<<22),
    };

    enum LayerType {
        kLayerAnimation = (1<<0),
        kLayerAnimColor = (1<<1),
        kLayerAnimAmbient = (1<<2),
        kLayerAnimScale = (1<<3),
        kLayer_10 = (1<<4),
        kLayerAnimUVW = (1<<5),
        kLayer_100 = (1<<8),
        kLayer_200 = (1<<9),
        kLayerRenderProcs = (1<<12),
        kLayerAVI = (1<<13),
        kLayer_4000 = (1<<14),
        kLayerAnimBehavior = (1<<15),
        kLayerQT = (1<<16),
        kLayerBink = (1<<17),
        kLayerWater = (1<<18),
        kLayerFire = (1<<19),
    };

public:
    imLayer(unsigned int type)
    : m_layerType(type), m_animScale(imAnimController::kTypeScalar),
      m_animAmbient(imAnimController::kTypeColor),
      m_animColor(imAnimController::kTypeColor),
      m_animUVW(imAnimController::kTypeMatrix44)
    { }

    virtual ~imLayer()
    { }

    static imLayer* MakeLayer(unsigned int type);

    virtual bool read(imStream* stream);
    virtual void readTextures(imStream* stream, class imSceneIndex* idx);

    unsigned int type() const
    { return m_layerType; }

    unsigned int blendFlags() const
    { return m_blendFlags; }

    unsigned int clampFlags() const
    { return m_clampFlags; }

    unsigned int shadeFlags() const
    { return m_shadeFlags; }

    unsigned int zFlags() const
    { return m_zFlags; }

    unsigned int miscFlags() const
    { return m_miscFlags; }

    float lodBias() const
    { return m_lodBias; }

protected:
    unsigned int m_layerType;
    unsigned int m_blendFlags, m_clampFlags, m_shadeFlags, m_zFlags, m_miscFlags;

    float m_lodBias;
    imColorRGBA m_color;
    imColorRGBA m_ambient;
    imMatrix44 m_transform;
    imAnimBehavior m_behavior;
    imAnimController m_animScale;
    imAnimController m_animAmbient;
    imAnimController m_animColor;
    imAnimController m_animUVW;
    std::vector<imRef<imTexture> > m_textures;
};

#endif
