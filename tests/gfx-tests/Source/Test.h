/****************************************************************************
Copyright (c) 2019-present Axmol Engine contributors (see AUTHORS.md).

https://axmolengine.github.io/

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
****************************************************************************/
#pragma once


#include <coroutine>
#include <doctest.h>
#include "base/RefPtr.h"
#include "platform/Image.h"


struct Empty{};


template <class T>
struct ValueHolder {
    bool hasValue = false;
    T value;
    void return_value(T&& value) { hasValue = true; this->value = std::forward<T>(value); }
    T&& get_value() { return std::forward<T>(value); }
};


template <>
struct ValueHolder<void> {
    bool hasValue = false;
    void return_void() { hasValue = true; }
    Empty get_value() { return {}; }
};


template <class T = void>
class [[nodiscard]] Task {
public:
    struct promise_type: public ValueHolder<T>
    {
        std::coroutine_handle<> _requester;

    public:
        std::suspend_never initial_suspend() noexcept { return {}; }
        auto final_suspend() noexcept {
            struct Awaiter {
                bool await_ready() const noexcept { return false; }
                auto await_suspend(std::coroutine_handle<promise_type> handle) noexcept
                {
                    auto requester = handle.promise()._requester;
                    if (!requester)
                        requester = std::noop_coroutine();
                    return requester;
                }

                void await_resume() const noexcept { }
            };

            return Awaiter{};
        }
        void unhandled_exception() {}
        Task<T> get_return_object() { return Task<T>(std::coroutine_handle<promise_type>::from_promise(*this)); }
    };

public:
    Task() {}

    void operator=(Task<T>&& other) {
        _handle = other._handle;
        other._handle = nullptr;
    }


    [[nodiscard]] auto operator co_await() &&
    {
        struct Awaiter {
            std::coroutine_handle<promise_type> handle;

            bool await_ready() { return handle.promise().hasValue; }
            void await_suspend(std::coroutine_handle<> requester) {
                handle.promise()._requester = requester;
            }
            [[nodiscard]] auto await_resume() { return handle.promise().get_value(); }
        };

        auto handle = _handle;
        _handle = nullptr;
        return Awaiter{ handle };
    }


private:
    Task(std::coroutine_handle<promise_type> handle): _handle(handle) {}

    std::coroutine_handle<promise_type> _handle;
};


//struct ImageCompareSettings
//{
//    float pixelError = 0.0f;
//};


struct ImageCompareResult
{
    int maxPixelError = 0;
    std::string error;
    ax::RefPtr<ax::Image> difference;
};


class GfxTests
{
public:
    static Task<ax::RefPtr<ax::Image>> captureFrame();

    static ax::RefPtr<ax::Image> loadImage(std::string_view path);
    static void saveImage(const ax::Image& image, std::string_view path);

    static ImageCompareResult compareImageToReference(const ax::Image* image, const ax::Image* reference);
};
