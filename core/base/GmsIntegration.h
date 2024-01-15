#pragma once

namespace ax { namespace internal {


using GmsEvent = void(*)();


struct GmsEventHooks {
    GmsEvent                    directorInitStart;
    GmsEvent                    directorInitEnd;

    GmsEvent                    frameStart;
    GmsEvent                    frameEnd;
    GmsEvent                    framePollEventsStart;
    GmsEvent                    framePollEventsEnd;
    GmsEvent                    frameUpdateStart;
    GmsEvent                    frameUpdateEnd;
    GmsEvent                    frameRenderStart;
    GmsEvent                    frameRenderEnd;

    GmsEvent                    beforeUpdateStart;
    GmsEvent                    beforeUpdateEnd;
    GmsEvent                    updateStart;
    GmsEvent                    updateEnd;
    GmsEvent                    runOnAxmolThreadStart;
    GmsEvent                    runOnAxmolThreadEnd;
    GmsEvent                    afterUpdateStart;
    GmsEvent                    afterUpdateEnd;

    GmsEvent                    drawClearStart;
    GmsEvent                    drawClearEnd;
    GmsEvent                    beforeDrawStart;
    GmsEvent                    beforeDrawEnd;
    GmsEvent                    drawStart;
    GmsEvent                    drawEnd;
    GmsEvent                    afterVisitStart;
    GmsEvent                    afterVisitEnd;
    GmsEvent                    renderStart;
    GmsEvent                    renderEnd;
    GmsEvent                    afterDrawStart;
    GmsEvent                    afterDrawEnd;

    GmsEvent                    appFinishLaunchingStart;
    GmsEvent                    appFinishLaunchingEnd;
    GmsEvent                    appToBackgroundStart;
    GmsEvent                    appToBackgroundEnd;
    GmsEvent                    appToForegroundStart;
    GmsEvent                    appToForegroundEnd;

    GmsEvent                    rendererBeginFrameStart;
    GmsEvent                    rendererBeginFrameEnd;
    GmsEvent                    rendererEndFrameStart;
    GmsEvent                    rendererEndFrameEnd;
    GmsEvent                    rendererRecreatedStart;
    GmsEvent                    rendererRecreated;
    GmsEvent                    rendererRecreatedEnd;
};


const GmsEventHooks& getGmsEventHooks();


}}
