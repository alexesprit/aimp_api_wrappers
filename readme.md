AIMP C++ API Wrappers
==
Wrappers for AIMP API written in C++ (like apiWrappers and AIMPCustomPlugin for Delphi).

Usage
---
Here is an example of wrapper usage:  

```cpp
/// File: MyAIMPPlugin.h
#pragma once
#include "AIMPCustomPlugin.h"

class MyAIMPPlugin : public AIMPCustomPlugin {
public:
    virtual HRESULT WINAPI Initialize(IAIMPCore* Core);
    virtual HRESULT WINAPI Finalize();

    virtual PWCHAR WINAPI InfoGet(int Index);
    virtual DWORD WINAPI InfoGetCategories();

    virtual void WINAPI SystemNotification(int NotifyID, IUnknown* Data);
};
```

```cpp
// File: MyAIMPPlugin.cpp
#include "MyAIMPPlugin.h"

HRESULT WINAPI MyAIMPPlugin::Initialize(IAIMPCore* Core) {
    // Base method call is required.
    if (SUCCEEDED(AIMPCustomPlugin::Initialize(Core))) {
        // Put your data initialization here.
        return S_OK;
    }
    return E_ABORT;
}

HRESULT WINAPI MyAIMPPlugin::Finalize() {
    // Base method call is required.
    if (SUCCEEDED(AIMPCustomPlugin::Finalize())) {
        // Put your data finalization here.
        return S_OK;
    }
    return E_ABORT;
}

// ...
```