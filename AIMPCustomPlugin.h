#pragma once

#include "AIMPCoreWrapper.h"
#include "IUnknownImpl.h"
#include "apiPlugin.h"

class AIMPCustomPlugin : public IUnknownImpl<IAIMPPlugin> {
public:
    virtual HRESULT WINAPI Initialize(IAIMPCore* Core);
    virtual HRESULT WINAPI Finalize();

protected:
    AIMPCoreWrapper* aimpCore;
};

