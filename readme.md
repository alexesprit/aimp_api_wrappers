# AIMP C++ API Wrappers
Wrappers for AIMP API written in C++ (like apiWrappers and AIMPCustomPlugin for Delphi).

## Table of contents
1. [Usage](#usage)
2. [Smart pointers](#smart-pointers)

### Usage
Here is an example of core wrapper usage:  

```cpp
/**
 * File: MyAIMPPlugin.h
 */

#pragma once
#include "AIMPCustomPlugin.h"

// Descendant of AIMPCustomPlugin
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
/**
 * File: MyAIMPPlugin.cpp
 */

#include "MyAIMPPlugin.h"

HRESULT WINAPI MyAIMPPlugin::Initialize(IAIMPCore* Core) {
    // Base method call is required.
    if (SUCCEEDED(AIMPCustomPlugin::Initialize(Core))) {
        // Okey, let's use one of help methods.
        IAIMPString* testString = aimpCore->MakeString(L"It works!");
        MessageBox(
            aimpCore->MainWindowGetHandle(),
            testString->GetData(), L"AIMP", MB_OK);
        testString->Release();
        // Put your data initialization here.
        return S_OK;
    }
    return E_ABORT;
}

HRESULT WINAPI MyAIMPPlugin::Finalize() {
    // Put your data finalization here.
    // Base method call is required.
    return AIMPCustomPlugin::Finalize();
}

// ...
```

### Smart pointers
Use smart COM object pointers:  
1. Include `comtypes.h` in your stdafx.h or in any file.  
2. Start using new types like in this example:  
```cpp
void MyPlugin::TestMethod() {
    IAIMPMenuItemPtr menuItem;
    if (aimpCore->CreateObject(IID_IAIMPMenuItem, (void**)&menuItem)) {
        IAIMPServiceMenuManagerPtr menuManager;
        if (aimpCore->GetService(IID_IAIMPServiceMenuManager, (void**)&menuManager)) {
            IAIMPMenuItemPtr parentMenuItem;
            if (SUCCEEDED(menuManager->GetBuiltIn(AIMP_MENUID_PLAYER_MAIN_OPEN, &parentMenuItem))) {
                IAIMPStringPtr menuName, menuId;
                aimpCore->MakeString(MENU_UPDATE_COLOR_SCHEME_ID, &menuId);
                aimpCore->LangLoadString(LNG_UPDATE_COLOR_SCHEME, &menuName);

                menuItem->SetValueAsObject(AIMP_MENUITEM_PROPID_ID, menuId);
                menuItem->SetValueAsObject(AIMP_MENUITEM_PROPID_NAME, menuName);
                menuItem->SetValueAsObject(AIMP_MENUITEM_PROPID_PARENT, parentMenuItem);
                menuItem->SetValueAsInt32(AIMP_MENUITEM_PROPID_STYLE, AIMP_MENUITEM_STYLE_NORMAL);

                aimpCore->RegisterExtension(IID_IAIMPServiceMenuManager, menuItem);
            }
        }
    }
} // All variables have leaved its scope, and COM objects instances will be removed automatically.
```
