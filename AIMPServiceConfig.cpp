#include "stdafx.h"
#include "AIMPServiceConfig.h"
#include "AIMPCoreWrapper.h"

AIMPServiceConfig::AIMPServiceConfig(AIMPCoreWrapper* wrapper) {
    aimpCore = wrapper;
    if (!aimpCore->GetService(IID_IAIMPServiceConfig, (void**)&configService)) {
        throw CONFIG_SERVICE_IS_NOT_SUPPORTED;
    }
}
AIMPServiceConfig::~AIMPServiceConfig() {
    configService->Release();
}

bool AIMPServiceConfig::ReadBool(PWCHAR keyPath, bool defaultValue) {
    return ReadInt32(keyPath, defaultValue ? 1 : 0) != 0;
}

double AIMPServiceConfig::ReadFloat(PWCHAR keyPath, double defaultValue) {
    auto result = defaultValue;
    auto sKeyPath = aimpCore->MakeString(keyPath);
    configService->GetValueAsFloat(sKeyPath, &result);
    sKeyPath->Release();
    return result;
}

INT64 AIMPServiceConfig::ReadInt64(PWCHAR keyPath, INT64 defaultValue) {
    auto result = defaultValue;
    auto sKeyPath = aimpCore->MakeString(keyPath);
    configService->GetValueAsInt64(sKeyPath, &result);
    sKeyPath->Release();
    return result;
}

int AIMPServiceConfig::ReadInt32(PWCHAR keyPath, int defaultValue) {
    auto result = defaultValue;
    auto sKeyPath = aimpCore->MakeString(keyPath);
    configService->GetValueAsInt32(sKeyPath, &result);
    sKeyPath->Release();
    return result;
}

IAIMPString* AIMPServiceConfig::ReadString(PWCHAR keyPath, PWCHAR defaultValue) {
    IAIMPString* result;
    auto sKeyPath = aimpCore->MakeString(keyPath);
    if (FAILED(configService->GetValueAsString(sKeyPath, &result))) {
        result = aimpCore->MakeString(defaultValue);
    }    
    sKeyPath->Release();
    return result;
}

IAIMPString* AIMPServiceConfig::ReadString(PWCHAR keyPath, IAIMPString* defaultValue) {
    IAIMPString* result;
    auto sKeyPath = aimpCore->MakeString(keyPath);
    if (FAILED(configService->GetValueAsString(sKeyPath, &result))) {
        result = defaultValue;
    }
    sKeyPath->Release();
    return result;
}

void AIMPServiceConfig::WriteBool(PWCHAR keyPath, bool keyValue) {
    WriteInt32(keyPath, keyValue ? 1 : 0);
}

void AIMPServiceConfig::WriteFloat(PWCHAR keyPath, double keyValue) {
    auto sKeyPath = aimpCore->MakeString(keyPath);
    configService->SetValueAsFloat(sKeyPath, keyValue);
    sKeyPath->Release();
}

void AIMPServiceConfig::WriteInt64(PWCHAR keyPath, INT64 keyValue) {
    auto sKeyPath = aimpCore->MakeString(keyPath);
    configService->SetValueAsInt64(sKeyPath, keyValue);
    sKeyPath->Release();
}

void AIMPServiceConfig::WriteInt32(PWCHAR keyPath, int keyValue) {
    auto sKeyPath = aimpCore->MakeString(keyPath);
    configService->SetValueAsInt32(sKeyPath, keyValue);
    sKeyPath->Release();
}

void AIMPServiceConfig::WriteString(PWCHAR keyPath, PWCHAR keyValue) {
    auto sKeyValue = aimpCore->MakeString(keyValue);
    WriteString(keyPath, sKeyValue);
    sKeyValue->Release();
}

void AIMPServiceConfig::WriteString(PWCHAR keyPath, IAIMPString* keyValue) {
    auto sKeyPath = aimpCore->MakeString(keyPath);
    configService->SetValueAsString(sKeyPath, keyValue);
    sKeyPath->Release();
}
