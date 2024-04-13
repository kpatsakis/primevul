bool PrintWebViewHelper::PrintPreviewContext::IsModifiable() {
  return !PrintingNodeOrPdfFrame(source_frame(), source_node_);
}
