﻿#pragma once

enum class LambertShadowMode : uint32_t
{
    ENABLE = 0,
    FORCE_DISABLE = 1,
    FORCE_ENABLE = 2
};

enum class FxaaIntensity : uint32_t
{
    DISABLED = 0,
    INTENSITY_0 = 1,
    INTENSITY_1 = 2,
    INTENSITY_2 = 3,
    INTENSITY_3 = 4,
    INTENSITY_4 = 5,
    INTENSITY_5 = 6,
    INTENSITY_6 = 7,
};

class Configuration
{
public:
    static bool fixBloomScale;
    static bool fixDofScale;
    static bool forceIgnoreFinalLightColorAdjustment;
    static FxaaIntensity fxaaIntensity;
    static bool postProcessingOnParticles;

    static float ambientShadowBiasObject;
    static float ambientShadowBiasTerrain;
    static bool enableTerrainShadowCast;
    static bool forceCastShadow;
    static bool forceDirectionalShadow;
    static LambertShadowMode lambertShadowObject;
    static LambertShadowMode lambertShadowTerrain;

    static bool enableResolutionScale;
    static int width;
    static int height;

    static bool load(const std::string& filePath);
};
