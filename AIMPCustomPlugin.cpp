#include "stdafx.h"
#include "AIMPCustomPlugin.h"

HRESULT WINAPI AIMPCustomPlugin::Initialize(IAIMPCore* core) {
    aimpCore = new AIMPCoreWrapper(core);
    return S_OK;
}

HRESULT WINAPI AIMPCustomPlugin::Finalize() {
    delete aimpCore;
    aimpCore = nullptr;
    return S_OK;
}
