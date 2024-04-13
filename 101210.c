bool PrintWebViewHelper::GetPrintFrame(blink::WebLocalFrame** frame) {
  DCHECK(frame);
  blink::WebView* webView = render_view()->GetWebView();
  DCHECK(webView);
  if (!webView)
    return false;

  blink::WebLocalFrame* focusedFrame =
      webView->focusedFrame()->toWebLocalFrame();
  *frame = focusedFrame->hasSelection()
               ? focusedFrame
               : webView->mainFrame()->toWebLocalFrame();
  return true;
}
