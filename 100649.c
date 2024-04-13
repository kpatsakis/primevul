void RenderFrameImpl::OnRedo() {
  frame_->executeCommand(WebString::fromUTF8("Redo"), GetFocusedElement());
}
