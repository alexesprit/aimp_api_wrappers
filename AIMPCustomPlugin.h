#pragma once

#include "apiPlugin.h"
#include "IUnknownImpl.h"

class AIMPCustomPlugin : public IUnknownImpl<IAIMPPlugin> {
public:
    virtual HRESULT WINAPI Initialize(IAIMPCore* Core);
    virtual HRESULT WINAPI Finalize();
protected:
    bool CreateObject(REFIID iid, void** object);
    HRESULT CreateObjectEx(REFIID iid, void** object);
    bool GetService(REFIID iid, void** service);
    bool RegisterExtension(REFIID iid, IUnknown* extension);

    PWCHAR CoreGetProfilePath();

    HRESULT MessageDispatcherSend(DWORD message, int param1, void* param2);
    HRESULT MessageDispatcherGetPropValue(int propertyId, void* buffer);
    HRESULT MessageDispatcherSetPropValue(int propertyId, void* value);

    HWND MainWindowGetHandle();

    IAIMPString* MakeString(PWCHAR strSeq);
    IAIMPString* MakeString(PWCHAR strSeq, int strSeqLen);
    HRESULT MakeString(PWCHAR strSeq, IAIMPString** out);
    HRESULT MakeString(PWCHAR strSeq, int strSeqLen, IAIMPString** out);

    IAIMPCore* aimpCore;
private:
    void CheckResult(HRESULT result, PWCHAR message);

    // Messages for CheckResult
    const PWCHAR UNABLE_TO_CREATE_IAIMPSTRING = L"Unable to create IAIMPString object";
    const PWCHAR UNABLE_TO_GET_PROFILE_PATH = L"Profile path is not found";
};

