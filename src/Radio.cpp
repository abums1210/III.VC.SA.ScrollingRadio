#include "..\includes\Radio.h"
#include "..\includes\RadioConfig.h"
#include "..\includes\InputHandler.h"
#include "..\includes\SomeMacros.h"
#include "extensions/Screen.h"
#include <cmath>
#include "CFileLoader.h"
#include "CMenuManager.h"
#include "CPad.h"
#include "CCamera.h"
#include "CReplay.h"

#ifdef GTA3
#include "cDMAudio.h"
#include "cMusicManager.h"
#include "eRadioStations.h"
#define RADIO_MANAGER gMusicManager
#define MUSIC_VOLUME_PREF CMenuManager::m_nPrefsMusicVolume
#define ACTIVE_STATIONS 12
#elif defined(GTAVC)
#include "cDMAudio.h"
#define RADIO_MANAGER DMAudio
#define MUSIC_VOLUME_PREF FrontEndMenuManager.m_nPrefsMusicVolume
#define ACTIVE_STATIONS 12
#else
#include "CAudioEngine.h"
#include "CAERadioTrackManager.h"
#include "eVehicleType.h"
#define RADIO_MANAGER AudioEngine
#define MUSIC_VOLUME_PREF FrontEndMenuManager.m_nPrefsMusicVolume
#define ACTIVE_STATIONS 13
#endif

using namespace plugin;

RwTexDictionary *Radio::radioTexDict = nullptr;

#ifdef GTA3
CSprite2d Radio::radioIcons[21];
#elif defined(GTAVC)
CSprite2d Radio::radioIcons[23];
int32_t& gNumRetunePresses = *(int32_t *)0x783998;
#else
CSprite2d Radio::radioIcons[25];
#endif

const char *Radio::radioIconNamesIII[21] = {
    "chatterbox", "chatterbox_gray", "doublecleff", "doublecleff_gray",
    "flashback",  "flashback_gray",  "gameradio",   "gameradio_gray",
    "headradio",  "headradio_gray",  "kjah",        "kjah_gray",
    "lips",       "lips_gray",       "mp3player",   "mp3player_gray",
    "msx",        "msx_gray",        "off_button",  "rise",
    "rise_gray"
};

const char *Radio::radioIconNamesVC[23] = {
    "810vcn",     "810vcn_gray", "emotion",      "emotion_gray", "espantoso",     "espantoso_gray",
    "fever",      "fever_gray",  "flash",        "flash_gray",   "kchat",         "kchat_gray",
    "mp3",        "mp3_gray",    "off_button",   "vcpr",         "vcpr_gray",     "vrock",
    "vrock_gray", "wave103",     "wave103_gray", "wildstyle",    "wildstyle_gray"
};

const char *Radio::radioIconNamesSA[25] = {"off_button",
                                           "radio_bounce",
                                           "radio_bounce_gray",
                                           "radio_csr",
                                           "radio_csr_gray",
                                           "radio_KDST",
                                           "radio_KDST_gray",
                                           "radio_kjah",
                                           "radio_kjah_gray",
                                           "radio_krose",
                                           "radio_krose_gray",
                                           "radio_mastersounds",
                                           "radio_mastersounds_gray",
                                           "radio_playback",
                                           "radio_playback_gray",
                                           "radio_RADIOX",
                                           "radio_RADIOX_gray",
                                           "radio_RLS",
                                           "radio_RLS_gray",
                                           "radio_SFUR",
                                           "radio_SFUR_gray",
                                           "radio_TPLAYER",
                                           "radio_TPLAYER_gray",
                                           "radio_WCTR",
                                           "radio_WCTR_gray"}; 

static int GetActiveStations(RadioStation *outStations) {
    int count;

    count = 0;
#ifdef GTA3
    outStations[count++] = { 0, 8, 9 };    // HEAD_RADIO -> headradio
    outStations[count++] = { 1, 2, 3 };    // DOUBLE_CLEF_FM -> doublecleff
    outStations[count++] = { 2, 10, 11 };  // JAH_RADIO -> kjah
    outStations[count++] = { 3, 19, 20 };  // RISE_FM -> rise
    outStations[count++] = { 4, 12, 13 };  // LIPS_106 -> lips
    outStations[count++] = { 5, 6, 7 };    // GAME_FM -> gameradio
    outStations[count++] = { 6, 16, 17 };  // MSX_FM -> msx
    outStations[count++] = { 7, 4, 5 };    // FLASHBACK_95_6 -> flashback
    outStations[count++] = { 8, 0, 1 };    // CHATTERBOX_109 -> chatterbox
    if (DMAudio.IsMP3RadioChannelAvailable()) {
        outStations[count++] = { 9, 14, 15 };  // MP3_PLAEYR -> mp3player
        outStations[count++] = { 11, 18, -1 }; // RADIO_OFF -> off_button
    } else {
        outStations[count++] = { 10, 18, -1 }; // RADIO_OFF -> off_button
    }
#elif defined(GTAVC)
    outStations[count++] = { 0, 21, 22 };  // WILDSTYLE
    outStations[count++] = { 1, 8, 9 };    // FLASH_FM
    outStations[count++] = { 2, 10, 11 };  // KCHAT
    outStations[count++] = { 3, 6, 7 };    // FEVER
    outStations[count++] = { 4, 17, 18 };  // V_ROCK
    outStations[count++] = { 5, 15, 16 };  // VCPR
    outStations[count++] = { 6, 4, 5 };    // RADIO_ESPANTOSO
    outStations[count++] = { 7, 2, 3 };    // EMOTION
    outStations[count++] = { 8, 19, 20 };  // WAVE
    if (RADIO_MANAGER.IsMP3RadioChannelAvailable()) {
        outStations[count++] = { 9, 12, 13 };  // MP3
        outStations[count++] = { 10, 14, -1 }; // RADIO_OFF (10)
    } else {
        outStations[count++] = { 9, 14, -1 };  // RADIO_OFF (9)
    }
#else
    outStations[count++] = {13, 0, -1}; // RADIO_OFF (13 is RADIO_NONE)
    outStations[count++] = {1, 13, 14}; // PLAYBACK
    outStations[count++] = {2, 9, 10};   // K_ROSE
    outStations[count++] = {3, 5, 6};    // K_DST
    outStations[count++] = {4, 1, 2};    // BOUNCE
    outStations[count++] = {5, 19, 20};  // SF_UR
    outStations[count++] = {6, 17, 18};  // RADIO_LOS_SANTOS
    outStations[count++] = {7, 15, 16};  // RADIO_X 
    outStations[count++] = {8, 3, 4};    // CSR 
    outStations[count++] = {9, 7, 8};    // JAH_RADIO
    outStations[count++] = {10, 11, 12};  // MASTERSOUNDS 
    outStations[count++] = {11, 23, 24};  // WCTR
    outStations[count++] = {12, 21, 22}; // MP3 
#endif
    return count;
}

static int GetScrollIndex(int gameId, const RadioStation *activeStations, int count) {
    int i;
    for (i = 0; i < count; i++) {
        if (activeStations[i].gameId == gameId) {
            return i;
        }
    }
    return count - 1;
}

static void DrawIcon(int index, float x, float y, float alpha, float relPos, const RadioStation *activeStations) {
    float finalWidth, finalHeight, absRel, colorFactor;
    int colorIdx, grayIdx, colorType;
    CRect rect;

    if (index < 0) {
        return;
    }

    finalWidth = SCREEN_SCALE_X(128.0f * (config.IconSizeMultiplier / 100.0f));
    finalHeight = SCREEN_SCALE_Y(128.0f * (config.IconSizeMultiplier / 100.0f));
    
    rect.left = x - finalWidth / 2.0f;
    rect.right = x + finalWidth / 2.0f;
    rect.top = y - finalHeight / 2.0f;
    rect.bottom = y + finalHeight / 2.0f;

    colorIdx = activeStations[index].colorIconIdx;
    grayIdx = activeStations[index].grayIconIdx;
    colorType = config.ColorType;

    if (colorType == 1) {
        Radio::radioIcons[colorIdx].Draw(rect, CRGBA(255, 255, 255, (unsigned char)alpha));
    } else if (colorType == 3) {
        if (grayIdx != -1) {
            Radio::radioIcons[grayIdx].Draw(rect, CRGBA(255, 255, 255, (unsigned char)alpha));
        } else {
            Radio::radioIcons[colorIdx].Draw(rect, CRGBA(150, 150, 150, (unsigned char)alpha));
        }
    } else {
        if (grayIdx == -1) {
            Radio::radioIcons[colorIdx].Draw(rect, CRGBA(255, 255, 255, (unsigned char)alpha));
        } else {
            absRel = fabs(relPos);
            if (absRel >= 1.0f) {
                Radio::radioIcons[grayIdx].Draw(rect, CRGBA(255, 255, 255, (unsigned char)alpha));
            } else {
                colorFactor = 1.0f - absRel;
                Radio::radioIcons[grayIdx].Draw(rect, CRGBA(255, 255, 255, (unsigned char)alpha));
                Radio::radioIcons[colorIdx].Draw(rect, CRGBA(255, 255, 255, (unsigned char)(alpha * colorFactor)));
            }
        }
    }
}

void Radio::InitRadio() {
    int i;
#ifdef GTA3
    radioTexDict = CFileLoader::LoadTexDictionary(GAME_PATH("charlesvercetti_mods\\ScrollingRadio\\radio_iii.txd"));
    for (i = 0; i < 21; ++i) {
        radioIcons[i].m_pTexture = RwTexDictionaryFindNamedTexture(radioTexDict, radioIconNamesIII[i]);
    }
#elif defined(GTAVC)
    radioTexDict = CFileLoader::LoadTexDictionary(GAME_PATH("charlesvercetti_mods\\ScrollingRadio\\radio_vc.txd"));
    for (i = 0; i < 23; ++i) {
        radioIcons[i].m_pTexture = RwTexDictionaryFindNamedTexture(radioTexDict, radioIconNamesVC[i]);
    }
#else
    radioTexDict = CFileLoader::LoadTexDictionary(GAME_PATH("charlesvercetti_mods\\ScrollingRadio\\radio_sa.txd"));
    for (i = 0; i < 25; ++i) {
        radioIcons[i].m_pTexture = RwTexDictionaryFindNamedTexture(radioTexDict, radioIconNamesSA[i]);
    }
#endif
}

void __fastcall Radio::DrawRadioIcons(void* self, int edx) {
    if (CTimer::m_UserPause || CTimer::m_CodePause || TheCamera.m_bWideScreenOn || CReplay::Mode) {
        return;
    }

    static float currentScrollPos = -1.0f;
    static float hudAlphaMultiplier = 0.0f;
    static int lastStationId = -1;
    static unsigned int displayTimeout = 0;
    static unsigned char savedVolume = 64;
    static bool isMuted = false;
    static bool wasDriving = false;
    CVehicle *vehicle;
    CPed *player;
    RadioStation activeStations[ACTIVE_STATIONS];
    int radioId, isDriving, targetScrollSlot, loopIndex, stationCount, rawRadioId, currentSlot, targetSlot, offset, stationsListed;
    float screenWidth, screenHeight, firstOffset, secondOffset, centerX, centerY, scrollDiff, interpStep, relPos, drawX, drawY, drawAlpha, alphaTarget, fadeSpeed;
    bool isRetuning, isHudActive;

    (void)offset;
    (void)stationsListed;

    vehicle = FindPlayerVehicle();
    player = FindPlayerPed();
    isDriving = player ? (player->m_ePedState == PEDSTATE_DRIVING) : 0;

    if (!isDriving) {
        wasDriving = false;
        return;
    }

#ifdef GTASA
    if (vehicle && vehicle->m_nVehicleSubClass == VEHICLE_BMX) {
        wasDriving = false;
        return;
    }
#endif

    stationCount = GetActiveStations(activeStations);

    // 1. Resolve raw game radio station ID and map ambient/silence to RADIO_OFF
#if defined(GTA3) || defined(GTAVC)
    rawRadioId = vehicle ? vehicle->m_nRadioStation : 0;
#ifdef GTA3
    if (rawRadioId == 12 || rawRadioId == 13) {
        rawRadioId = DMAudio.IsMP3RadioChannelAvailable() ? 11 : 10;
    }
#else // VC
    if (rawRadioId >= 11 && rawRadioId <= 14) {
        rawRadioId = RADIO_MANAGER.IsMP3RadioChannelAvailable() ? 10 : 9;
    }
#endif

    // 2. Map raw ID to contiguous slot index, apply preview offset, then resolve final radioId
    currentSlot = GetScrollIndex(rawRadioId, activeStations, stationCount);
    targetSlot = currentSlot;
    if (gNumRetunePresses != 0) {
        offset = gNumRetunePresses;
#ifdef GTAVC
        if (!RADIO_MANAGER.IsMP3RadioChannelAvailable()) {
            if (offset >= 2) {
                offset -= 1;
            } else if (offset <= -2) {
                offset += 1;
            }
        }
#endif
        targetSlot = currentSlot + offset;
        while (targetSlot < 0) targetSlot += stationCount;
        while (targetSlot >= stationCount) targetSlot -= stationCount;
    }
    radioId = activeStations[targetSlot].gameId;

    // 3. Handle quick mute volume saving & restoration
    if (isMuted) {
        if (CPad::GetPad(0)->ChangeStationJustDown()) {
            DMAudio.SetMusicMasterVolume(savedVolume);
            isMuted = false;
        }
    }

    // Quick mute button 'X' handling
    if (InputHandler::IsKeyJustPressed('X')) {
        savedVolume = MUSIC_VOLUME_PREF;
        DMAudio.SetMusicMasterVolume(0);
        isMuted = true;
#ifdef GTA3
        DMAudio.SetRadioInCar(10);
        vehicle->m_nRadioStation = DMAudio.IsMP3RadioChannelAvailable() ? 11 : 10;
#else
        if (DMAudio.IsMP3RadioChannelAvailable()) {
            DMAudio.SetRadioInCar(10);
            vehicle->m_nRadioStation = 10;
        } else {
            DMAudio.SetRadioInCar(9);
            vehicle->m_nRadioStation = 9;
        }
#endif
    }
#else // SA
    if (AERadioTrackManager.m_bDisplayStationName && AERadioTrackManager.IsVehicleRadioActive()) {
        AERadioTrackManager.m_nTimeToDisplayRadioName = CTimer::m_snTimeInMilliseconds + 2500;
        AERadioTrackManager.m_bDisplayStationName = false;
    }

    rawRadioId = 13;
    if (vehicle && AERadioTrackManager.IsVehicleRadioActive()) {
        char radioType = vehicle->m_vehicleAudio.m_settings.m_nRadioType;
        if (radioType != RADIO_EMERGENCY && radioType != RADIO_DISABLED) {
            rawRadioId = RADIO_MANAGER.GetCurrentRadioStationID();
        }
    }
    currentSlot = GetScrollIndex(rawRadioId, activeStations, stationCount);
    targetSlot = currentSlot;
    stationsListed = AERadioTrackManager.m_nStationsListed;
    if (stationsListed != 0) {
        targetSlot = currentSlot + stationsListed;
        while (targetSlot < 0) targetSlot += stationCount;
        while (targetSlot >= stationCount) targetSlot -= stationCount;
    }
    radioId = activeStations[targetSlot].gameId;

    if (isMuted) {
        if (CPad::GetPad(0)->NextStationJustUp() || CPad::GetPad(0)->LastStationJustUp()) {
            RADIO_MANAGER.SetMusicMasterVolume(savedVolume);
            isMuted = false;
        }
    }

    if (InputHandler::IsKeyJustPressed('X')) {
        savedVolume = MUSIC_VOLUME_PREF;
        RADIO_MANAGER.SetMusicMasterVolume(1);
        isMuted = true;
    }
#endif

    // Auto-update display timeout if radio station changes or player just entered a car
    if (!wasDriving || radioId != lastStationId) {
        displayTimeout = CTimer::m_snTimeInMilliseconds + config.RadioSliderTimeout;
        lastStationId = radioId;
        wasDriving = true;
    }

    // Keep scrolling animation running even if HUD is not shown to keep it in sync
    targetScrollSlot = GetScrollIndex(radioId, activeStations, stationCount);
    if (currentScrollPos < 0.0f) {
        currentScrollPos = (float)targetScrollSlot;
    }

    isRetuning = false;
#if defined(GTA3) || defined(GTAVC)
    isRetuning = (gNumRetunePresses != 0);
#else
    isRetuning = (stationsListed != 0);
#endif

    if (config.DisableScrollAnim || (config.OnlyPlayScrollOnPress && !isRetuning)) {
        currentScrollPos = (float)targetScrollSlot;
    }

    scrollDiff = (float)targetScrollSlot - currentScrollPos;
    while (scrollDiff < -((float)stationCount / 2.0f)) {
        scrollDiff += (float)stationCount;
    }
    while (scrollDiff > ((float)stationCount / 2.0f)) {
        scrollDiff -= (float)stationCount;
    }

    if (fabs(scrollDiff) > 0.001f) {
        interpStep = ((float)config.RadioSliderAnimSpeed / 100.0f) * CTimer::ms_fTimeStep;
        if (interpStep > 1.0f) {
            interpStep = 1.0f;
        }
        currentScrollPos += scrollDiff * interpStep;
        while (currentScrollPos < 0.0f) {
            currentScrollPos += (float)stationCount;
        }
        while (currentScrollPos >= (float)stationCount) {
            currentScrollPos -= (float)stationCount;
        }
    } else {
        currentScrollPos = (float)targetScrollSlot;
    }

    // Fade In / Fade Out animation handling
    isHudActive = ((unsigned int)CTimer::m_snTimeInMilliseconds <= displayTimeout);
    alphaTarget = isHudActive ? 1.0f : 0.0f;
    fadeSpeed = 0.08f * CTimer::ms_fTimeStep;
    if (hudAlphaMultiplier < alphaTarget) {
        hudAlphaMultiplier += fadeSpeed;
        if (hudAlphaMultiplier > alphaTarget) {
            hudAlphaMultiplier = alphaTarget;
        }
    } else if (hudAlphaMultiplier > alphaTarget) {
        hudAlphaMultiplier -= fadeSpeed;
        if (hudAlphaMultiplier < alphaTarget) {
            hudAlphaMultiplier = alphaTarget;
        }
    }

    // Render the icons if the HUD is visible (alpha > 0.0f)
    if (hudAlphaMultiplier > 0.0f) {
        screenHeight = screen::GetScreenMaxY();
        screenWidth = screen::GetScreenMaxX();
        
        // Calculate offsets and positions
        secondOffset = screenHeight / (float)config.IconGap;
        firstOffset = secondOffset / 2.0f;

        centerX = screenWidth / 2.0f;
        centerY = screenHeight / 12.0f;

        RwRenderStateSet(rwRENDERSTATESHADEMODE, (void *)rwSHADEMODEGOURAUD);
        RwRenderStateSet(rwRENDERSTATETEXTUREFILTER, (void *)rwFILTERLINEARMIPLINEAR);
        RwRenderStateSet(rwRENDERSTATEVERTEXALPHAENABLE, (void *)TRUE);

        for (loopIndex = 0; loopIndex < stationCount; loopIndex++) {
            relPos = (float)loopIndex - currentScrollPos;
            while (relPos < -((float)stationCount / 2.0f)) {
                relPos += (float)stationCount;
            }
            while (relPos > ((float)stationCount / 2.0f)) {
                relPos -= (float)stationCount;
            }

            if (config.CenterIconOnly && fabs(relPos) > 0.99f) {
                continue;
            }

            if (fabs(relPos) <= 2.5f) {
                drawX = centerX + relPos * firstOffset;
                drawY = centerY;

                drawAlpha = 0.0f;
                if (fabs(relPos) <= 1.0f) {
                    drawAlpha = 192.0f - fabs(relPos) * (192.0f - (float)config.Alpha1stNonActive);
                } else if (fabs(relPos) <= 2.0f) {
                    drawAlpha = (float)config.Alpha1stNonActive - (fabs(relPos) - 1.0f) * ((float)config.Alpha1stNonActive - (float)config.Alpha2ndNonActive);
                } else if (fabs(relPos) <= 2.5f) {
                    drawAlpha = (float)config.Alpha2ndNonActive - (fabs(relPos) - 2.0f) * 2.0f * (float)config.Alpha2ndNonActive;
                }

                drawAlpha *= hudAlphaMultiplier;
                DrawIcon(loopIndex, drawX, drawY, drawAlpha, relPos, activeStations);
            }
        }
    }
}

void Radio::ShutdownRadio() {
    int i;
    if (radioTexDict) {
#ifdef GTA3
        for (i = 0; i < 21; ++i) {
            radioIcons[i].m_pTexture = nullptr;
        }
#elif defined(GTAVC)
        for (i = 0; i < 23; ++i) {
            radioIcons[i].m_pTexture = nullptr;
        }
#else
        for (i = 0; i < 25; ++i) {
            radioIcons[i].m_pTexture = nullptr;
        }
#endif
        radioTexDict = nullptr;
    }
}
