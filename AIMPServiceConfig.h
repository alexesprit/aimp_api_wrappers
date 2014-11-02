#pragma once

#include "apiCore.h"

// Forward declaration
class AIMPCoreWrapper;

class AIMPServiceConfig {
public:
    AIMPServiceConfig(AIMPCoreWrapper* wrapper);
    ~AIMPServiceConfig();

    bool ReadBool(PWCHAR keyPath, bool defaultValue);
    double ReadFloat(PWCHAR keyPath, double defaultValue);
    INT64 ReadInt64(PWCHAR keyPath, INT64 defaultValue);
    int ReadInt32(PWCHAR keyPath, int defaultValue);
    IAIMPString* ReadString(PWCHAR keyPath, PWCHAR defaultValue);
    IAIMPString* ReadString(PWCHAR keyPath, IAIMPString* defaultValue);

    void WriteBool(PWCHAR keyPath, bool keyValue);
    void WriteFloat(PWCHAR keyPath, double keyValue);
    void WriteInt64(PWCHAR keyPath, INT64 keyValue);
    void WriteInt32(PWCHAR keyPath, INT32 keyValue);
    void WriteString(PWCHAR keyPath, PWCHAR keyValue);
    void WriteString(PWCHAR keyPath, IAIMPString* keyValue);

private:
    const PWCHAR CONFIG_SERVICE_IS_NOT_SUPPORTED = L"The IAIMPServiceConfig is not supported";

    IAIMPServiceConfig* configService;
    AIMPCoreWrapper* aimpCore;
};
