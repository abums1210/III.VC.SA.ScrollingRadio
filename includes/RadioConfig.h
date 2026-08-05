#pragma once
#include "ConfigHelper.h"

class RadioConfig {
  public:
    int ReloadKey;
    int ColorType;
    int IconGap;
    int IconSizeMultiplier;
    int Alpha1stNonActive;
    int Alpha2ndNonActive;
    int RadioSliderTimeout;
    int RadioSliderAnimSpeed;
    int OnlyPlayScrollOnPress;
    int DisableScrollAnim;
    int CenterIconOnly;

    RadioConfig();

    bool Read();
    void Write();
    void GenerateDefault();
};
extern RadioConfig config;