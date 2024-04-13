bool RenderView::allowPlugins(WebFrame* frame, bool enabled_per_settings) {
  if (CommandLine::ForCurrentProcess()->HasSwitch(
      switches::kEnableClickToPlay)) {
    return WebFrameClient::allowPlugins(frame, enabled_per_settings);
  }
  return (enabled_per_settings &&
      AllowContentType(CONTENT_SETTINGS_TYPE_PLUGINS));
}
