static WorkQueue& pluginWorkQueue()
{
    DEFINE_STATIC_LOCAL(WorkQueue, queue, ("com.apple.CoreIPC.PluginQueue"));
    return queue;
}
