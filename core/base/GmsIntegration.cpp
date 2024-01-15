#include "GmsIntegration.h"


static void doNothing() {}

#ifdef _MSC_VER
#pragma comment(linker, "/alternatename:_initGmsEventHooks=_initGmsEventHooksDefault")   
extern "C" ax::internal::GmsEventHooks initGmsEventHooks();
extern "C" ax::internal::GmsEventHooks initGmsEventHooksDefault() {
#else
__attribute__ ((weak))
extern "C" ax::internal::GmsEventHooks initGmsEventHooks() {
#endif
    return ax::internal::GmsEventHooks {
        .directorInitStart = doNothing,
        .directorInitEnd = doNothing,

        .frameStart = doNothing,
        .frameEnd = doNothing,
        .framePollEventsStart = doNothing,
        .framePollEventsEnd = doNothing,
        .frameUpdateStart = doNothing,
        .frameUpdateEnd = doNothing,
        .frameRenderStart = doNothing,
        .frameRenderEnd = doNothing,

        .beforeUpdateStart = doNothing,
        .beforeUpdateEnd = doNothing,
        .updateStart = doNothing,
        .updateEnd = doNothing,
        .runOnAxmolThreadStart = doNothing,
        .runOnAxmolThreadEnd = doNothing,
        .afterUpdateStart = doNothing,
        .afterUpdateEnd = doNothing,

        .drawClearStart = doNothing,
        .drawClearEnd = doNothing,
        .beforeDrawStart = doNothing,
        .beforeDrawEnd = doNothing,
        .drawStart = doNothing,
        .drawEnd = doNothing,
        .afterVisitStart = doNothing,
        .afterVisitEnd = doNothing,
        .renderStart = doNothing,
        .renderEnd = doNothing,
        .afterDrawStart = doNothing,
        .afterDrawEnd = doNothing,

        .appFinishLaunchingStart = doNothing,
        .appFinishLaunchingEnd = doNothing,
        .appToBackgroundStart = doNothing,
        .appToBackgroundEnd = doNothing,
        .appToForegroundStart = doNothing,
        .appToForegroundEnd = doNothing,

        .rendererBeginFrameStart = doNothing,
        .rendererBeginFrameEnd = doNothing,
        .rendererEndFrameStart = doNothing,
        .rendererEndFrameEnd = doNothing,
        .rendererRecreatedStart = doNothing,
        .rendererRecreatedEnd = doNothing,
    };
}


namespace ax { namespace internal {


const GmsEventHooks& getGmsEventHooks() {
    static auto hooks = initGmsEventHooks();
    return hooks;
}


}} // namespace ax::internal
