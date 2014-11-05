#pragma once

template <typename T> class IUnknownImpl : public T {
public:
    IUnknownImpl() : refCount(0) {
    }

    virtual HRESULT WINAPI QueryInterface(REFIID riid, void** ppvObject) {
        if (ppvObject == nullptr) {
            return E_INVALIDARG;
        }

        if (IID_IUnknown == riid) {
            *ppvObject = static_cast<IUnknown*>(this);
        } else if (__uuidof(this) == riid) {
            *ppvObject = static_cast<T*>(this);
        } else {
            *ppvObject = nullptr;
            return E_NOINTERFACE;
        }

        AddRef();
        return S_OK;
    }

    virtual ULONG WINAPI AddRef() {
        return InterlockedIncrement(&refCount);
    }

    virtual ULONG WINAPI Release() {
        ULONG currentRefCount = InterlockedDecrement(&refCount);
        if (currentRefCount == 0) {
            delete this;
        }
        return currentRefCount;
    }

private:
    ULONG refCount;
};