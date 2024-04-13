bool RenderView::supportsFullscreen() {
  return CommandLine::ForCurrentProcess()->HasSwitch(
      switches::kEnableVideoFullscreen);
}
