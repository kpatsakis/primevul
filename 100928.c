void RenderViewImpl::OnScrollFocusedEditableNodeIntoRect(
    const gfx::Rect& rect) {
  WebKit::WebNode node = GetFocusedNode();
  if (!node.isNull()) {
    if (IsEditableNode(node)) {
      webview()->saveScrollAndScaleState();
      webview()->scrollFocusedNodeIntoRect(rect);
    }
  }
}
