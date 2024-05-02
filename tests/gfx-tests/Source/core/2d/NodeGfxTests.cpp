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


TEST_SUITE("core/2d/Node") {
    TEST_CASE("Sorting") {
        static std::promise<void> promise;

        GfxTestRunner::runTest([](Scene* scene) -> Task<> {
            constexpr auto start = 32.0f;
            constexpr auto shift = (480.0f - 32.0f * 2.0f) / 9.0f;
            constexpr auto y1 = 220.0f;

            auto create = [](int x) -> Sprite* {
                auto sprite = Sprite::create("common/filled" + std::to_string(x) + ".png");
                sprite->setName(std::to_string(x));
                return sprite;
            };

            auto setPos = [](Sprite* sprite, int line, int x, int y) {
                sprite->setWorldPosition(Vec2(start + shift * x, y1 - 20.0f * y));
            };

            {
                auto sprite0 = create(0);
                auto sprite1 = create(1);
                auto sprite2 = create(2);
                auto sprite3 = create(3);
                auto sprite4 = create(4);
                auto sprite5 = create(5);
                auto sprite6 = create(6);
                auto sprite7 = create(7);
                auto sprite8 = create(8);
                auto sprite9 = create(9);

                scene->addChild(sprite9); // global z = 2
                scene->addChild(sprite3);
                {
                    sprite3->addChild(sprite8); // global z = 1
                    sprite3->addChild(sprite4);
                    sprite3->addChild(sprite7); // local z = 1
                    sprite3->addChild(sprite5);
                    sprite3->addChild(sprite2); // local z = -1
                    sprite3->addChild(sprite6);
                    sprite3->addChild(sprite1); // global z = -1
                }
                scene->addChild(sprite0); // global z = -2

                sprite0->setGlobalZOrder(-2);
                sprite1->setGlobalZOrder(-1);
                sprite2->setLocalZOrder(-1);
                sprite7->setLocalZOrder(1);
                sprite8->setGlobalZOrder(1);
                sprite9->setGlobalZOrder(2);

                setPos(sprite0, 0, 0, 0);
                setPos(sprite3, 0, 3, 0);
                {
                    setPos(sprite1, 0, 1, 1);
                    setPos(sprite2, 0, 2, 1);
                    setPos(sprite4, 0, 4, 1);
                    setPos(sprite5, 0, 5, 1);
                    setPos(sprite6, 0, 6, 1);
                    setPos(sprite7, 0, 7, 1);
                    setPos(sprite8, 0, 8, 1);
                }
                setPos(sprite9, 0, 9, 0);
            }


            auto frame = co_await GfxTests::captureFrame();
            auto reference = GfxTests::loadImage("core/2d/Node_Sorting.png");
            auto comparison = GfxTests::compareImageToReference(frame, reference);
            GfxTests::saveImage(*frame, "results/core/2d/Node_Sorting.png");
            if (not comparison.error.empty() && comparison.difference)
                GfxTests::saveImage(*comparison.difference, "results/core/2d/Node_Sorting_diff.png");

            CHECK_EQ(comparison.error, "");

            promise.set_value();
        });

        promise.get_future().wait();
    }
}
