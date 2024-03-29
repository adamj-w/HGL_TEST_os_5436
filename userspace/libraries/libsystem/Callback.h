#pragma once

#error "This doesn't work rn"

#include <libsystem/OwnPtr.h>
#include <libsystem/Move.h>
#include <libsystem/Assert.h>

namespace hegel {

template<typename>
class Callback;

template<typename Out, typename... In>
class Callback<Out(In...)>
{
private:
    class AbstractCallbackWrapper
    {
    public: 
        virtual ~AbstractCallbackWrapper() {}

        virtual Out call(In...) const = 0;
    };

    template<typename T>
    class CallbackWrapper final : public AbstractCallbackWrapper
    {
    private:
        T _callback;

    public:
        CallbackWrapper(T&& callback) : _callback(callback) {}
        CallbackWrapper(const CallbackWrapper&) = delete;
        CallbackWrapper& operator=(const CallbackWrapper&) = delete;

        Out call(In... in) const
        {
            return _callback(forward<In>(in)...);
        }
    };

    OwnPtr<AbstractCallbackWrapper> _wrapper;

public:
    template<typename CallbackType>
    Callback(CallbackType&& callback) : _wrapper(own<CallbackWrapper<CallbackType>>(callback)) {}

    ~Callback() {}

    Out operator()(In... in) const
    {
        assert(_wrapper);
        return _wrapper->call(forward<In>(in)...);
    }

    template<typename CallbackType>
    Callback& operator=(CallbackType&& callback)
    {
        _wrapper = own<CallbackWrapper<CallbackType>>(callback);
        return *this;
    }

    operator bool() const { return !!_wrapper; }
};

}