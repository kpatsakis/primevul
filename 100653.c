void RenderFrameImpl::didChangeContentsSize(blink::WebLocalFrame* frame,
                                            const blink::WebSize& size) {
  DCHECK(!frame_ || frame_ == frame);
#if defined(OS_MACOSX)
  if (frame->parent())
    return;

  WebView* frameView = frame->view();
  if (!frameView)
    return;

  GetRenderWidget()->DidChangeScrollbarsForMainFrame(
      frame->hasHorizontalScrollbar(),
      frame->hasVerticalScrollbar());
#endif  // defined(OS_MACOSX)
}
