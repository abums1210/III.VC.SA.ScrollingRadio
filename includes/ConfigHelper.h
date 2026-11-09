#pragma once
#include <plugin.h>
#include <string>
#include <vector>
#include <map>
#include <cstdint>

class ConfigHelper {
    std::string path;
    std::map<std::string, std::string> comments; // "section|key" -> comment
    void StoreComment(const char* sec, const char* key, const char* comment);
public:
    ConfigHelper(const std::string& iniPath);

    int ReadInt(const char* sec, const char* key, int def);
    float ReadFloat(const char* sec, const char* key, float def);
    CVector2D ReadVec(const char* sec, const char* key, CVector2D def);
    CRect ReadRect(const char* sec, const char* key, CRect def);
    CRGBA ReadRGBA(const char* sec, const char* key, CRGBA def);
    std::string ReadString(const char* sec, const char* key, const std::string& def);
    int ReadIntArray(const char* sec, const char* key, int16_t* out, int count, const int16_t* def);

    void WriteInt(const char* sec, const char* key, int val, const char* comment = nullptr);
    void WriteFloat(const char* sec, const char* key, float val, const char* comment = nullptr);
    void WriteVec(const char* sec, const char* key, CVector2D val, const char* comment = nullptr);
    void WriteRect(const char* sec, const char* key, CRect val, const char* comment = nullptr);
    void WriteRGBA(const char* sec, const char* key, CRGBA val, const char* comment = nullptr);
    void WriteString(const char* sec, const char* key, const std::string& val, const char* comment = nullptr);
    void WriteIntArray(const char* sec, const char* key, const int16_t* arr, int count, const char* comment = nullptr);

    bool Exists();
    void Format(const std::string& headerComments = "");
};
