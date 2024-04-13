void RenderViewTest::LoadHTMLWithUrlOverride(const char* html,
                                             const char* url_override) {
  GetMainFrame()->LoadHTMLString(std::string(html),
                                 blink::WebURL(GURL(url_override)));
  FrameLoadWaiter(view_->GetMainRenderFrame()).Wait();
  view_->GetWebView()->UpdateAllLifecyclePhases();
}
