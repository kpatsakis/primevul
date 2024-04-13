WebFrame* GetFrameForResource(const Resource* resource) {
  PluginInstance* plugin_instance = ResourceHelper::GetPluginInstance(resource);
  if (!plugin_instance)
    return NULL;
  return plugin_instance->container()->element().document().frame();
}
