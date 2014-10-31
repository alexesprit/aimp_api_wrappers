#include "stdafx.h"
#include "apiMessages.h"
#include "apiMUI.h"
#include "AIMPCustomPlugin.h"

HRESULT WINAPI AIMPCustomPlugin::Initialize(IAIMPCore* Core) {
    aimpCore = Core;
    aimpCore->AddRef();
    return S_OK;
}

HRESULT WINAPI AIMPCustomPlugin::Finalize() {
    aimpCore->Release();
    aimpCore = nullptr;
    return S_OK;
}

bool AIMPCustomPlugin::CreateObject(REFIID iid, void** object) {
    return aimpCore->CreateObject(iid, object) == S_OK;
}

HRESULT AIMPCustomPlugin::CreateObjectEx(REFIID iid, void** object) {
    return aimpCore->CreateObject(iid, object);
}

bool AIMPCustomPlugin::GetService(REFIID iid, void** service) {
    return aimpCore->QueryInterface(iid, service) == S_OK;
}

bool AIMPCustomPlugin::RegisterExtension(REFIID iid, IUnknown* extension) {
    return aimpCore->RegisterExtension(iid, extension) == S_OK;
}

PWCHAR AIMPCustomPlugin::CoreGetProfilePath() {
    IAIMPString* profilePath;
    if (FAILED(aimpCore->GetPath(AIMP_CORE_PATH_PROFILE, &profilePath))) {
        throw UNABLE_TO_GET_PROFILE_PATH;
    }
    PWCHAR resultValue = profilePath->GetData();
    profilePath->Release();
    return resultValue;
}

IAIMPString* AIMPCustomPlugin::LangGetName() {
    IAIMPServiceMUI* muiService;
    if (GetService(IID_IAIMPServiceMUI, (void**)&muiService)) {
        IAIMPString* langName;
        muiService->GetName(&langName);
        muiService->Release();
        return langName;
    }
    return MakeString(L"");
}

IAIMPString* AIMPCustomPlugin::LangGetValue(PWCHAR keyPath) {
    IAIMPString* value;
    if (SUCCEEDED(LangLoadString(keyPath, &value))) {
        return value;
    }
    return MakeString(L"");
}
IAIMPString* AIMPCustomPlugin::LangGetValue(PWCHAR keyPath, int partIndex) {
    IAIMPString* value;
    if (SUCCEEDED(LangLoadString(keyPath, partIndex, &value))) {
        return value;
    }
    return MakeString(L"");
}

HRESULT AIMPCustomPlugin::MessageDispatcherSend(DWORD message, int param1, void* param2) {
    IAIMPServiceMessageDispatcher* messageService;
    if (GetService(IID_IAIMPServiceMessageDispatcher, (void**)&messageService)) {
        messageService->Send(message, param1, param2);
        messageService->Release();
        return S_OK;
    }
    return E_UNEXPECTED;
}

HRESULT AIMPCustomPlugin::MessageDispatcherGetPropValue(int propertyId, void* buffer) {
    return MessageDispatcherSend(propertyId, AIMP_MSG_PROPVALUE_GET, buffer);
}
HRESULT AIMPCustomPlugin::MessageDispatcherSetPropValue(int propertyId, void* value) {
    return MessageDispatcherSend(propertyId, AIMP_MSG_PROPVALUE_SET, value);
}

HWND AIMPCustomPlugin::MainWindowGetHandle() {
    HWND aimpHwnd = 0;
    MessageDispatcherGetPropValue(AIMP_MSG_PROPERTY_HWND, &aimpHwnd);
    return aimpHwnd;
}

IAIMPString* AIMPCustomPlugin::MakeString(PWCHAR strSeq) {
    return MakeString(strSeq, wcslen(strSeq));
}

IAIMPString* AIMPCustomPlugin::MakeString(PWCHAR strSeq, int strSeqLen) {
    IAIMPString* string;
    CreateObject(IID_IAIMPString, (void**)&string);
    CheckResult(string->SetData(strSeq, strSeqLen), UNABLE_TO_CREATE_IAIMPSTRING);
    return string;
}

HRESULT AIMPCustomPlugin::MakeString(PWCHAR strSeq, IAIMPString** out) {
    try {
        *out = MakeString(strSeq);
        return S_OK;
    } catch (...) {
        return E_NOINTERFACE;
    }
}

HRESULT AIMPCustomPlugin::MakeString(PWCHAR strSeq, int strSeqLen, IAIMPString** out) {
    try {
        *out = MakeString(strSeq, strSeqLen);
        return S_OK;
    } catch (...) {
        return E_NOINTERFACE;
    }
}

bool AIMPCustomPlugin::PropListGetStr(IAIMPPropertyList* propList, int propId, IAIMPString** out) {
    return SUCCEEDED(propList->GetValueAsObject(propId, IID_IAIMPString, (void**)out));
}

void AIMPCustomPlugin::CheckResult(HRESULT result, PWCHAR message) {
    if (result != S_OK) {
        throw message;
    }
}

HRESULT AIMPCustomPlugin::LangLoadString(PWCHAR keyPath, IAIMPString** out) {
    IAIMPServiceMUI* muiService;
    if (GetService(IID_IAIMPServiceMUI, (void**)&muiService)) {
        IAIMPString* keyPath_ = MakeString(keyPath);
        HRESULT result = muiService->GetValue(keyPath_, out);
        keyPath_->Release();
        muiService->Release();
        return result;
    }
    return E_NOINTERFACE;
}

HRESULT AIMPCustomPlugin::LangLoadString(PWCHAR keyPath, int partIndex, IAIMPString** out) {
    IAIMPServiceMUI* muiService;
    if (GetService(IID_IAIMPServiceMUI, (void**)&muiService)) {
        IAIMPString* keyPath_ = MakeString(keyPath);
        HRESULT result = muiService->GetValuePart(keyPath_, partIndex, out);
        keyPath_->Release();
        muiService->Release();
        return result;
    }
    return E_NOINTERFACE;
}
