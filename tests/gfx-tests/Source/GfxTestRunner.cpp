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

#if AX_TARGET_PLATFORM == AX_PLATFORM_ANDROID
    #include <android/log.h>
    #include <sstream>
#endif
#include <string>
#include <doctest.h>
#include "base/Director.h"
#include "base/RefPtr.h"
#include "GfxTestRunner.h"
#include "Test.h"

USING_NS_AX;


static std::function<void(RefPtr<Image>&&)> gCaptureFrameRequester;
static Task<void> gTest;


void GfxTestRunner::setupTesting() {
    auto director = Director::getInstance();
    auto renderer = director->getRenderer();
    auto dispatcher = director->getEventDispatcher();

    // !!!Metal: needs setFrameBufferOnly before draw
    #if defined(AX_USE_METAL)
        auto captureEvent = Director::EVENT_BEFORE_DRAW;
    #else
        auto captureEvent = Director::EVENT_AFTER_DRAW;
    #endif

    dispatcher->addCustomEventListener(captureEvent, [=](EventCustom*) {
        if (gCaptureFrameRequester) {
            // Schedule capturing of the frame during this frame
            renderer->readPixels(renderer->getDefaultRenderTarget(), [=](const backend::PixelBufferDescriptor& pbd) {
                if (!pbd)
                    throw std::runtime_error("Failed to read pixels");

                auto image = utils::makeInstance<Image>(&Image::initWithRawData, pbd._data.getBytes(),
                    pbd._data.getSize(), pbd._width, pbd._height, 8, false);

                // Calling the requester might request another frame capture, so grab the requester
                auto requester = std::move(gCaptureFrameRequester);
                gCaptureFrameRequester = nullptr;

                requester(std::forward<RefPtr<Image>>(image));
            });
        }
    });
}


#if AX_TARGET_PLATFORM == AX_PLATFORM_ANDROID
    // Redirect stdout to logcat
    class AndroidLogStream: public std::stringbuf
    {
    public:
        virtual int sync() {
            auto data = str();
            auto lines = std::string_view(data);
            auto newLine = std::string_view::size_type(0);

            // Print each line separately
            while ((newLine = lines.find('\n')) != std::string_view::npos) {
                __android_log_print(ANDROID_LOG_INFO, "gfx-tests", "%.*s", int(newLine), lines.data());
                lines.remove_prefix(newLine + 1);
            }

            str(std::string(lines));

            return 0;
        }
    };
#endif


void GfxTestRunner::startTests() {
    std::thread([] {
        doctest::Context context;

        #if AX_TARGET_PLATFORM == AX_PLATFORM_ANDROID
            // On Android, redirect stdout to logcat
            auto out = AndroidLogStream();
            auto stream = std::ostream(&out);
            context.setCout(&stream);
        #endif

        // TODO: pass in command line arguments
        context.applyCommandLine(0, 0);

        //context.addFilter("test-case-exclude", "*math*"); // exclude test cases with "math" in their name
        //context.setOption("abort-after", 5);              // stop test execution after 5 failed assertions
        context.setOption("no-breaks", true);             // don't break in the debugger when assertions fail

        int res = context.run(); // run

        #if AX_TARGET_PLATFORM == AX_PLATFORM_ANDROID
            // Make sure all output is printed before exiting
            stream.flush();
            out.pubsync();
        #endif

        Director::getInstance()->getScheduler()->runOnAxmolThread([res] {
            exit(res);
        });
    }).detach();
}


void GfxTestRunner::runTest(std::function<Task<>(Scene* scene)> testStarter) {
    Director::getInstance()->getScheduler()->runOnAxmolThread([=] {
        auto scene = Scene::create();
        gTest = testStarter(scene);
        Director::getInstance()->replaceScene(scene);
    });
}


Task<RefPtr<Image>> GfxTests::captureFrame() {
    struct awaitable
    {
        RefPtr<Image>* _value;
        std::coroutine_handle<> _requester;

        bool await_ready() { return false; }
        void await_suspend(std::coroutine_handle<> requester)
        {
            if (gCaptureFrameRequester)
                throw std::runtime_error("captureFrame() is already in progress");
            _requester = requester;
            gCaptureFrameRequester = [this](RefPtr<Image>&& image) {
                _value = std::addressof(image);
                _requester.resume();
            };
        }
        RefPtr<Image>&& await_resume() { return std::forward<RefPtr<Image>>(*_value); }

        //std::function<void()> func;
    };

    co_return co_await awaitable{};
}


ax::RefPtr<ax::Image> GfxTests::loadImage(std::string_view path) {
    auto image = RefPtr<Image>();
    auto fullPath = FileUtils::getInstance()->fullPathForFilename(path);
    if (!fullPath.empty())
        image = utils::makeInstance<Image>(&Image::initWithImageFile, path);
    return image;
}


void GfxTests::saveImage(const ax::Image& image, std::string_view path) {
    auto fullPath = std::string();

    if (not FileUtils::getInstance()->isAbsolutePath(path)) {
        fullPath = FileUtils::getInstance()->getWritablePath() + std::string(path);
        path = fullPath;
    }

    auto dir = path.substr(0, path.find_last_of('/'));
    if (!FileUtils::getInstance()->createDirectory(dir))
        throw std::runtime_error("Failed to create directory: " + std::string(dir));

    if (!image.saveToFile(path, false))
        throw std::runtime_error("Failed to save image: " + std::string(path));
}


ImageCompareResult GfxTests::compareImageToReference(const ax::Image* image, const ax::Image* reference) {
    auto result = ImageCompareResult();

    if (image == nullptr)
    {
        result.error = "Image is null";
    }
    else if (reference == nullptr)
    {
        result.error = "Reference image is null";
    }
    else if (image->getWidth() != reference->getWidth() || image->getHeight() != reference->getHeight())
    {
        result.error = "Image sizes do not match";
    }
    else if (image->getPixelFormat() != backend::PixelFormat::RGBA8)
    {
        result.error = "Image format is not RGBA8";
    }
    else if (reference->getPixelFormat() != backend::PixelFormat::RGBA8)
    {
        result.error = "Reference image format is not RGBA8";
    }
    else
    {
        auto width = image->getWidth();
        auto height = image->getHeight();
        auto sizeInBytes = width * height * 4;
        auto diffStart = new uint8_t[sizeInBytes];
        auto diffEnd = diffStart + sizeInBytes;
        auto imageData = image->getData();
        auto referenceData = reference->getData();

        GP_ASSERT(image->getDataLen() == sizeInBytes);
        GP_ASSERT(reference->getDataLen() == sizeInBytes);

        for (auto diff = diffStart; diff < diffEnd; diff += 4)
        {
            auto r = uint8_t(std::abs(imageData[0] - referenceData[0]));
            auto g = uint8_t(std::abs(imageData[1] - referenceData[1]));
            auto b = uint8_t(std::abs(imageData[2] - referenceData[2]));
            auto a = uint8_t(std::abs(imageData[3] - referenceData[3]));

            diff[0] = std::max(r, a);
            diff[1] = std::max(g, a);
            diff[2] = std::max(b, a);
            diff[3] = 255;

            auto error = r + g + b + a;
            result.maxPixelError = std::max(result.maxPixelError, error);

            imageData += 4;
            referenceData += 4;
        }

        result.difference = utils::makeInstance<Image>(
            &Image::initWithRawData, diffStart, sizeInBytes, width, height, 8, false);
    }

    return result;
}
