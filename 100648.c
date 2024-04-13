void RenderFrameImpl::OnDelete() {
  frame_->executeCommand(WebString::fromUTF8("Delete"), GetFocusedElement());
}
