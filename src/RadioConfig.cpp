#include "../includes/RadioConfig.h"
#include <cstdlib>
#include <fstream>
#include <string>

#define RADIO_CONFIG_INI_PATH GAME_PATH("charlesvercetti_mods\\ScrollingRadio\\scrolling_radio.ini")

RadioConfig config;

RadioConfig::RadioConfig() {
    ReloadKey = 162; // VK_LCONTROL
    ColorType = 2;   // 1. Fully color, 2. active in color, others grey, 3. all grey
    IconGap = 3;
    IconSizeMultiplier = 50; // 100%
    Alpha1stNonActive = 128;   // 64 by default
    Alpha2ndNonActive = 64;   // 32 by default
    RadioSliderTimeout = 4000;
    RadioSliderAnimSpeed = 15;
    OnlyPlayScrollOnPress = 0;
    DisableScrollAnim = 0;
    CenterIconOnly = 0;
    EnableBicycleRadioIcon = 0;
    DisableIconDisplayWhileEntering = 0;
}

bool RadioConfig::Read() {
    std::string iniPath = RADIO_CONFIG_INI_PATH;
    ConfigHelper cfg(iniPath);

    if (!cfg.Exists()) {
        return false;
    }

    ReloadKey = cfg.ReadInt("Settings", "ReloadKey", ReloadKey);
    ColorType = cfg.ReadInt("Settings", "ColorType", ColorType);
    IconGap = cfg.ReadInt("Settings", "IconGap", IconGap);
    IconSizeMultiplier = cfg.ReadInt("Settings", "IconSizeMultiplier", IconSizeMultiplier);
    Alpha1stNonActive = cfg.ReadInt("Settings", "Alpha1stNonActive", Alpha1stNonActive);
    Alpha2ndNonActive = cfg.ReadInt("Settings", "Alpha2ndNonActive", Alpha2ndNonActive);
    RadioSliderTimeout = cfg.ReadInt("Settings", "RadioSliderTimeout", RadioSliderTimeout);
    RadioSliderAnimSpeed = cfg.ReadInt("Settings", "RadioSliderAnimSpeed", RadioSliderAnimSpeed);
    OnlyPlayScrollOnPress = cfg.ReadInt("Settings", "OnlyPlayScrollOnPress", OnlyPlayScrollOnPress);
    DisableScrollAnim = cfg.ReadInt("Settings", "DisableScrollAnim", DisableScrollAnim);
    CenterIconOnly = cfg.ReadInt("Settings", "CenterIconOnly", CenterIconOnly);
    EnableBicycleRadioIcon = cfg.ReadInt("Settings", "EnableBicycleRadioIcon", EnableBicycleRadioIcon);
    DisableIconDisplayWhileEntering = cfg.ReadInt("Settings", "DisableIconDisplayWhileEntering", DisableIconDisplayWhileEntering);

    return true;
}

void RadioConfig::Write() {
    std::string iniPath = RADIO_CONFIG_INI_PATH;
    ConfigHelper cfg(iniPath);

    cfg.WriteInt("Settings", "ReloadKey", ReloadKey, "Key to reload configuration");
    cfg.WriteInt("Settings", "ColorType", ColorType, "1 = Fully color, 2 = Active color / others grey, 3 = All grey");
    cfg.WriteInt("Settings", "IconGap", IconGap, "Gap divisor between icons");
    cfg.WriteInt("Settings", "IconSizeMultiplier", IconSizeMultiplier, "Size multiplier in percentage (e.g. 100 for 1.0x)");
    cfg.WriteInt("Settings", "Alpha1stNonActive", Alpha1stNonActive, "Transparency level of the 1st non-active icon (0-255)");
    cfg.WriteInt("Settings", "Alpha2ndNonActive", Alpha2ndNonActive, "Transparency level of the 2nd non-active icon (0-255)");
    cfg.WriteInt("Settings", "RadioSliderTimeout", RadioSliderTimeout, "Display duration in milliseconds");
    cfg.WriteInt("Settings", "RadioSliderAnimSpeed", RadioSliderAnimSpeed, "Scroll animation speed factor (1-100, where 15 = 0.15)");
    cfg.WriteInt("Settings", "OnlyPlayScrollOnPress", OnlyPlayScrollOnPress, "Play scroll slide animation only on button press (0 = false, 1 = true)");
    cfg.WriteInt("Settings", "DisableScrollAnim", DisableScrollAnim, "Disable scroll slide animation entirely (0 = false, 1 = true)");
    cfg.WriteInt("Settings", "CenterIconOnly", CenterIconOnly, "Display center icon only, hiding side icons (0 = false, 1 = true)");
    cfg.WriteInt("Settings", "EnableBicycleRadioIcon", EnableBicycleRadioIcon, "Enable radio display on bicycles in SA (0 = false, 1 = true)");
    cfg.WriteInt("Settings", "DisableIconDisplayWhileEntering", DisableIconDisplayWhileEntering, "Disable radio icons display when entering a vehicle (0 = false, 1 = true)");

    cfg.Format("; scrolling_radio.ini - Scrolling Radio Configuration\n");
}

void RadioConfig::GenerateDefault() {
    std::string iniPath = RADIO_CONFIG_INI_PATH;
    ConfigHelper cfg(iniPath);

    if (!cfg.Exists()) {
        Write();
    }
}
