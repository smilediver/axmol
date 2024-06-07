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

#include <string>

#include "2d/FontFreeType.h"
#include "platform/FileUtils.h"
#include "platform/GLView.h"
#include "platform/PlatformConfig.h"

#if AX_TARGET_PLATFORM == AX_PLATFORM_IOS
    #include "platform/ios/GLViewImpl-ios.h"
#endif

#include "GfxTestRunner.h"
#include "AppDelegate.h"


USING_NS_AX;

static const Vec2 gTestViewSize = Vec2(480, 270);


void setupFileUtils()
{
    auto fu = FileUtils::getInstance();

    #if AX_TARGET_PLATFORM == AX_PLATFORM_OSX
        fu->setWritablePath(fu->getWritablePath() + "gfx-tests/");
    #endif

    #if AX_TARGET_PLATFORM == AX_PLATFORM_WIN32 || AX_TARGET_PLATFORM == AX_PLATFORM_LINUX
        fu->setDefaultResourceRootPath(fu->getAppRoot() + "Content");
    #endif

    auto resourcesPath = fu->getDefaultResourceRootPath();
    fu->setSearchPaths({
        resourcesPath, // For finding resources
        resourcesPath + "tests/default", // For finding test reference images
    });

    fu->removeDirectory(fu->getWritablePath() + "results");
}


void AppDelegate::initGLContextAttrs()
{
    // set OpenGL context attributes: red,green,blue,alpha,depth,stencil
    GLContextAttrs glContextAttrs = {8, 8, 8, 8, 24, 8, 0};

    GLView::setGLContextAttrs(glContextAttrs);
}


bool AppDelegate::applicationDidFinishLaunching()
{
    setupFileUtils();

    // Enable logging output colored text style and prefix timestamp
    ax::setLogFmtFlag(ax::LogFmtFlag::Full);

    // whether enable global SDF font render support, since axmol-2.0.1
    FontFreeType::setShareDistanceFieldEnabled(true);

    // initialize director
    auto director = Director::getInstance();
    auto glView   = director->getGLView();
    if (!glView)
    {
        std::string title = "Gfx Tests";
#ifdef NDEBUG
        title += " (Release)",
#else
        title += " (Debug)",
#endif
#ifdef AX_PLATFORM_PC
        #if AX_TARGET_PLATFORM == AX_PLATFORM_OSX
            glView = GLViewImpl::createWithRect(title, Rect(0, 0, gTestViewSize.x, gTestViewSize.y), 0.5f, false, false);
        #else
            glView = GLViewImpl::createWithRect(title, Rect(0, 0, gTestViewSize.x, gTestViewSize.y), 1.0f, false, true);
        #endif
#else
        glView = GLViewImpl::createWithRect(title, Rect(0, 0, gTestViewSize.x, gTestViewSize.y));
#endif
        director->setGLView(glView);
    }

#ifdef AX_PLATFORM_PC
    director->setAnimationInterval(1.0f / glfwGetVideoMode(glfwGetPrimaryMonitor())->refreshRate);
#else
    director->setAnimationInterval(1.0f / 60);
#endif

    auto screenSize = glView->getFrameSize();

    glView->setDesignResolutionSize(gTestViewSize.width, gTestViewSize.height, ResolutionPolicy::SHOW_ALL);

    GfxTestRunner::setupTesting();
    GfxTestRunner::startTests();

    return true;
}


void AppDelegate::applicationDidEnterBackground()
{
    Director::getInstance()->stopAnimation();
}


void AppDelegate::applicationWillEnterForeground()
{
    Director::getInstance()->startAnimation();
}
