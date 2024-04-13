void RenderViewImpl::InstrumentDidCancelFrame() {
  if (!webview())
    return;
  if (!webview()->devToolsAgent())
    return;
  webview()->devToolsAgent()->didCancelFrame();
}
