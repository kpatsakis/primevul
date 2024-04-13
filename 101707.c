void PDFiumEngine::LoadPages() {
  if (pages_.empty()) {
    if (!doc_loader_->IsDocumentComplete()) {
      CheckPageAvailable(FPDFAvail_GetFirstPageNum(doc_), &pending_pages_);
    }
    LoadPageInfo(false);
  }
}
