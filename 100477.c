void PPB_URLLoader_Impl::RegisterCallback(
    scoped_refptr<TrackedCallback> callback) {
  DCHECK(!TrackedCallback::IsPending(pending_callback_));

  PluginModule* plugin_module = ResourceHelper::GetPluginModule(this);
  if (!plugin_module)
    return;

  pending_callback_ = callback;
}
