NS_DestroyPluginInstance(nsPluginInstanceBase* aPlugin)
{
    delete static_cast<gnash::nsPluginInstance *> (aPlugin);
}
