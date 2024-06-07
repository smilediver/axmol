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
#include <future>
#include "core/base/Director.h"
#include "core/2d/Scene.h"
#include "core/2d/Sprite.h"
#include "Test.h"
#include "GfxTestRunner.h"

using namespace ax;


TEST_SUITE("core/2d/Sprite") {
    TEST_CASE("Basic") {
        static std::promise<void> promise;

        GfxTestRunner::runTest([](Scene* scene) -> Task<> {
            auto x1 = 48.0f + 96.0f * 0.0f;
            auto x2 = 48.0f + 96.0f * 1.0f;
            auto x3 = 48.0f + 96.0f * 2.0f;
            auto x4 = 48.0f + 96.0f * 3.0f;
            auto x5 = 48.0f + 96.0f * 4.0f;

            auto y1 = 48.0f + 96.0f * 0.0f;
            auto y2 = 48.0f + 96.0f * 1.0f;

            {
                auto sprite = Sprite::create("common/filled1.png");
                scene->addChild(sprite);
                sprite->setPosition(Vec2(x1, y1));
            }

            {
                auto sprite = Sprite::create("common/filled2.png");
                scene->addChild(sprite);
                sprite->setPosition(Vec2(x2, y1));
                sprite->setRotation(45);
            }

            {
                auto sprite = Sprite::create("common/filled3.png");
                scene->addChild(sprite);
                sprite->setPositionNormalized(Vec2(x3 / 480.0f, y1 / 270.0f));
            }

            {
                auto sprite = Sprite::create("common/filled4.png");
                scene->addChild(sprite);
                sprite->setPositionNormalized(Vec2(x4 / 480.0f, y1 / 270.0f));
                sprite->setRotation(45);
            }

            {
                auto sprite = Sprite::create("common/filled5.png");
                scene->addChild(sprite);
                sprite->setPosition(Vec2(x5, y1));
                sprite->setScale(0.75f, 0.5f);
            }

            {
                auto sprite = Sprite::create("common/filled6.png");
                scene->addChild(sprite);
                sprite->setPosition(Vec2(x1, y2));
                sprite->setSkewX(20.0f);
            }

            {
                auto sprite = Sprite::create("common/filled7.png");
                scene->addChild(sprite);
                sprite->setPosition(Vec2(x2, y2));
                sprite->setSkewY(10.0f);
            }

            {
                auto sprite = Sprite::create("common/filled8.png");
                scene->addChild(sprite);
                sprite->setPosition(Vec2(x3, y2));
                sprite->setSkewX(20.0f);
                sprite->setSkewY(10.0f);
            }

            auto frame = co_await GfxTests::captureFrame();
            auto reference = GfxTests::loadImage("core/2d/Sprite_Basic.png");
            auto comparison = GfxTests::compareImageToReference(frame, reference);
            GfxTests::saveImage(*frame, "results/core/2d/Sprite_Basic.png");

            CHECK_LE(comparison.maxPixelError, 3);

            CHECK_EQ(comparison.error, "");
            if (not comparison.error.empty() && comparison.difference)
                GfxTests::saveImage(*comparison.difference, "results/core/2d/Sprite_Basic_diff.png");

            promise.set_value();
        });

        promise.get_future().wait();
    }


    TEST_CASE("PixelPerfect") {
        static std::promise<void> promise;

        GfxTestRunner::runTest([](Scene* scene) -> Task<> {
            {
                auto sprite = Sprite::create("common/pattern_rgb_border.png");
                scene->addChild(sprite);

                auto texture = sprite->getTexture();
                texture->setTexParameters(backend::SamplerDescriptor(
                    backend::SamplerFilter::NEAREST,
                    backend::SamplerFilter::NEAREST,
                    backend::SamplerAddressMode::REPEAT,
                    backend::SamplerAddressMode::REPEAT
                ));

                sprite->setAnchorPoint(Vec2(0, 0));
                sprite->setTexture(texture);
                sprite->setTextureRect(Rect(0, 0, 480, 270));
            }

            auto frame = co_await GfxTests::captureFrame();
            auto reference = GfxTests::loadImage("core/2d/Sprite_PixelPerfect.png");
            auto comparison = GfxTests::compareImageToReference(frame, reference);
            GfxTests::saveImage(*frame, "results/core/2d/Sprite_PixelPerfect.png");

            CHECK_LE(comparison.maxPixelError, 0);

            CHECK_EQ(comparison.error, "");
            if (not comparison.error.empty() && comparison.difference)
                GfxTests::saveImage(*comparison.difference, "results/core/2d/Sprite_PixelPerfect_diff.png");

            promise.set_value();
        });

        promise.get_future().wait();
    }
}
