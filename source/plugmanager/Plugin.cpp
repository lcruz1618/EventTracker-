#include "Plugin.h"
#include "EventTracker.h"

#if defined __cplusplus
extern "C" {
#endif

PLUGIN_API bool initB (BankManager& manager, const OCFILE_PATH& path)
{
    BankManager::RegParams rp;

    rp.PluginInfo.plugin = PLUGIN_NAME;
    rp.PluginInfo.type = PLUGIN_TYPE;
    rp.PluginInfo.version = PLUGIN_VERSION;
    rp.PluginInfo.path = path;

    rp.PluginCreate = EventTracker::EventTrackerCreate;
    rp.PluginDestroy = EventTracker::EventTrackerDestroy;

    return manager.RegisterPlugin (rp);
}

#if defined __cplusplus
}
#endif
