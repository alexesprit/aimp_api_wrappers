#pragma once

#include "apiCore.h"

// Forward declaration
class AIMPServiceConfig;

class AIMPCoreWrapper {
public:
    AIMPCoreWrapper(IAIMPCore* core);
    ~AIMPCoreWrapper();

    HRESULT WINAPI QueryInterface(REFIID iid, void** object);
    HRESULT WINAPI GetPath(int PathID, IAIMPString **Value);

    bool CreateObject(REFIID iid, void** object);
    bool GetService(REFIID iid, void** service);
    bool RegisterService(IUnknown *Service);
    bool RegisterExtension(REFIID iid, IUnknown* extension);

    IAIMPString* GetProfilePath();

    IAIMPString* LangGetName();
    IAIMPString* LangGetValue(PWCHAR keyPath);
    IAIMPString* LangGetValue(PWCHAR keyPath, int partIndex);

    HRESULT MessageDispatcherSend(DWORD message, int param1, void* param2);
    HRESULT MessageDispatcherGetPropValue(int propertyId, void* buffer);
    HRESULT MessageDispatcherSetPropValue(int propertyId, void* value);

    HWND MainWindowGetHandle();

    IAIMPString* MakeString(PWCHAR strSeq);
    IAIMPString* MakeString(PWCHAR strSeq, int strSeqLen);
    HRESULT MakeString(PWCHAR strSeq, IAIMPString** out);
    HRESULT MakeString(PWCHAR strSeq, int strSeqLen, IAIMPString** out);

    bool PropListGetStr(IAIMPPropertyList* propList, int propId, IAIMPString** out);
private:
    IAIMPCore* aimpCore;

    void CheckResult(HRESULT result, PWCHAR message);

    HRESULT LangLoadString(PWCHAR keyPath, IAIMPString** out);
    HRESULT LangLoadString(PWCHAR keyPath, int partIndex, IAIMPString** out);

    // Messages for CheckResult
    const PWCHAR UNABLE_TO_CREATE_IAIMPSTRING = L"Unable to create IAIMPString object";
    const PWCHAR UNABLE_TO_GET_PROFILE_PATH = L"Profile path is not found";
};