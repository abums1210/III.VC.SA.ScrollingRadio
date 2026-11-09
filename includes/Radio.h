#pragma once
#include "plugin.h"
#include "CSprite2d.h"

struct RadioStation {
    int gameId;
    int colorIconIdx;
    int grayIconIdx;
};

class Radio {
  public:
    static RwTexDictionary *radioTexDict;
#ifdef GTA3
    static CSprite2d radioIcons[22];
    static const char *radioIconNamesIII[22];
#elif defined(GTAVC)
    static CSprite2d radioIcons[24];
    static const char *radioIconNamesVC[24];
#else
    static CSprite2d radioIcons[26];
    static const char *radioIconNamesSA[26];
#endif

    static void InitRadio();
    static void __fastcall DrawRadioIcons(void* self = nullptr, int edx = 0);
    static void ShutdownRadio();
};