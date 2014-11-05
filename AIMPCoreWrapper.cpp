#include "stdafx.h"
#include "AIMPCoreWrapper.h"
#include "AIMPServiceConfig.h"
#include "apiMessages.h"
#include "apiMUI.h"

AIMPCoreWrapper::AIMPCoreWrapper(IAIMPCore* core) {
    aimpCore = core;
    aimpCore->AddRef();
}
AIMPCoreWrapper::~AIMPCoreWrapper() {
    aimpCore->Release();
    aimpCore = nullptr;
}

HRESULT WINAPI AIMPCoreWrapper::QueryInterface(REFIID iid, void** object) {
    return aimpCore->QueryInterface(iid, object);
}

HRESULT WINAPI AIMPCoreWrapper::GetPath(int pathId, IAIMPString** value) {
    return aimpCore->GetPath(pathId, value);
}

bool AIMPCoreWrapper::RegisterService(IUnknown* service) {
    return SUCCEEDED(aimpCore->RegisterService(service));
}

bool AIMPCoreWrapper::CreateObject(REFIID iid, void** object) {
    return SUCCEEDED(aimpCore->CreateObject(iid, object));
}

bool AIMPCoreWrapper::GetService(REFIID iid, void** service) {
    return SUCCEEDED(aimpCore->QueryInterface(iid, service));
}

bool AIMPCoreWrapper::IsServiceAvailable(REFIID riid) {
    IUnknown* dummyService;
    if (SUCCEEDED(aimpCore->QueryInterface(riid, (void**)&dummyService))) {
        dummyService->Release();
        return true;
    }
    return false;
}

bool AIMPCoreWrapper::RegisterExtension(REFIID iid, IUnknown* extension) {
    return SUCCEEDED(aimpCore->RegisterExtension(iid, extension));
}

AIMPServiceConfig* AIMPCoreWrapper::CreateServiceConfig() {
    return new AIMPServiceConfig(this);
}

IAIMPString* AIMPCoreWrapper::GetProfilePath() {
    IAIMPString* profilePath;
    CheckResult(aimpCore->GetPath(AIMP_CORE_PATH_PROFILE, &profilePath), UNABLE_TO_GET_PROFILE_PATH);
    return profilePath;
}

IAIMPString* AIMPCoreWrapper::LangGetName() {
    IAIMPServiceMUIPtr muiService;
    if (GetService(IID_IAIMPServiceMUI, (void**)&muiService)) {
        IAIMPString* langName;
        muiService->GetName(&langName);
        return langName;
    }
    return MakeString(L"");
}

IAIMPString* AIMPCoreWrapper::LangGetValue(PWCHAR keyPath) {
    IAIMPString* value;
    if (SUCCEEDED(LangLoadString(keyPath, &value))) {
        return value;
    }
    return MakeString(L"");
}
IAIMPString* AIMPCoreWrapper::LangGetValue(PWCHAR keyPath, int partIndex) {
    IAIMPString* value;
    if (SUCCEEDED(LangLoadString(keyPath, partIndex, &value))) {
        return value;
    }
    return MakeString(L"");
}

HRESULT AIMPCoreWrapper::MessageDispatcherSend(DWORD message, int param1, void* param2) {
    IAIMPServiceMessageDispatcherPtr messageService;
    if (GetService(IID_IAIMPServiceMessageDispatcher, (void**)&messageService)) {
        messageService->Send(message, param1, param2);
        return S_OK;
    }
    return E_UNEXPECTED;
}

HRESULT AIMPCoreWrapper::MessageDispatcherGetPropValue(int propertyId, void* buffer) {
    return MessageDispatcherSend(propertyId, AIMP_MSG_PROPVALUE_GET, buffer);
}
HRESULT AIMPCoreWrapper::MessageDispatcherSetPropValue(int propertyId, void* value) {
    return MessageDispatcherSend(propertyId, AIMP_MSG_PROPVALUE_SET, value);
}

HWND AIMPCoreWrapper::MainWindowGetHandle() {
    HWND aimpHwnd = 0;
    MessageDispatcherGetPropValue(AIMP_MSG_PROPERTY_HWND, &aimpHwnd);
    return aimpHwnd;
}

IAIMPString* AIMPCoreWrapper::MakeString(PWCHAR strSeq) {
    return MakeString(strSeq, wcslen(strSeq));
}

IAIMPString* AIMPCoreWrapper::MakeString(PWCHAR strSeq, int strSeqLen) {
    IAIMPString* string;
    CheckResult(aimpCore->CreateObject(IID_IAIMPString, (void**)&string), UNABLE_TO_CREATE_IAIMPSTRING);
    CheckResult(string->SetData(strSeq, strSeqLen), UNABLE_TO_CREATE_IAIMPSTRING);
    return string;
}

HRESULT AIMPCoreWrapper::MakeString(PWCHAR strSeq, IAIMPString** out) {
    return MakeString(strSeq, wcslen(strSeq), out);
}

HRESULT AIMPCoreWrapper::MakeString(PWCHAR strSeq, int strSeqLen, IAIMPString** out) {
    HRESULT result = aimpCore->CreateObject(IID_IAIMPString, (void**)out);
    if (SUCCEEDED(result)) {
        return (*out)->SetData(strSeq, wcslen(strSeq));
    }
    return result;

}

bool AIMPCoreWrapper::PropListGetStr(IAIMPPropertyList* propList, int propId, IAIMPString** out) {
    return SUCCEEDED(propList->GetValueAsObject(propId, IID_IAIMPString, (void**)out));
}

void AIMPCoreWrapper::CheckResult(HRESULT result, PWCHAR message) {
    if (FAILED(result)) {
        throw message;
    }
}

HRESULT AIMPCoreWrapper::LangLoadString(PWCHAR keyPath, IAIMPString** out) {
    IAIMPServiceMUIPtr muiService;
    if (GetService(IID_IAIMPServiceMUI, (void**)&muiService)) {
        IAIMPStringPtr sKeyPath;
        MakeString(keyPath, &sKeyPath);
        return muiService->GetValue(sKeyPath, out);
    }
    return E_NOINTERFACE;
}

HRESULT AIMPCoreWrapper::LangLoadString(PWCHAR keyPath, int partIndex, IAIMPString** out) {
    IAIMPServiceMUIPtr muiService;
    if (GetService(IID_IAIMPServiceMUI, (void**)&muiService)) {
        IAIMPStringPtr sKeyPath;
        MakeString(keyPath, &sKeyPath);
        return muiService->GetValuePart(sKeyPath, partIndex, out);
    }
    return E_NOINTERFACE;
}
