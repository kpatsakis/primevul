void PrintWebViewHelper::OnPrintForSystemDialog() {
  WebFrame* frame = print_preview_context_.frame();
  if (!frame) {
    NOTREACHED();
    return;
  }

  Print(frame, print_preview_context_.node());
}
