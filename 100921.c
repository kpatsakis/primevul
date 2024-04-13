RenderView* RenderView::FromWebView(WebKit::WebView* webview) {
  return RenderViewImpl::FromWebView(webview);
}
