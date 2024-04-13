void RenderViewImpl::OnImeConfirmComposition(
      const string16& text, const ui::Range& replacement_range) {
  if (pepper_helper_->IsPluginFocused()) {
    pepper_helper_->OnImeConfirmComposition(text);
  } else {
#if defined(OS_WIN)
    if (focused_plugin_id_ >= 0) {
      std::set<WebPluginDelegateProxy*>::iterator it;
      for (it = plugin_delegates_.begin();
           it != plugin_delegates_.end(); ++it) {
        (*it)->ImeCompositionCompleted(text, focused_plugin_id_);
      }
      return;
    }
#endif
    if (replacement_range.IsValid() && webview()) {
      if (WebFrame* frame = webview()->focusedFrame()) {
        WebRange webrange = WebRange::fromDocumentRange(
            frame, replacement_range.start(), replacement_range.length());
        if (!webrange.isNull())
          frame->selectRange(webrange);
      }
    }
    RenderWidget::OnImeConfirmComposition(text, replacement_range);
  }
}
