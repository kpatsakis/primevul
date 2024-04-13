void PDFiumEngine::OnPendingRequestComplete() {
  if (!process_when_pending_request_complete_)
    return;
  if (!fpdf_availability_) {
    file_access_.m_FileLen = doc_loader_->GetDocumentSize();
    fpdf_availability_ = FPDFAvail_Create(&file_availability_, &file_access_);
    DCHECK(fpdf_availability_);
    if (FPDFAvail_IsLinearized(fpdf_availability_) != PDF_LINEARIZED) {
      process_when_pending_request_complete_ = false;
      FPDFAvail_Destroy(fpdf_availability_);
      fpdf_availability_ = nullptr;
      return;
    }
  }

  if (!doc_) {
    LoadDocument();
    return;
  }

  if (pages_.empty()) {
    LoadBody();
    return;
  }

  bool update_pages = false;
  std::vector<int> still_pending;
  for (int pending_page : pending_pages_) {
    if (CheckPageAvailable(pending_page, &still_pending)) {
      update_pages = true;
      if (IsPageVisible(pending_page)) {
        client_->NotifyPageBecameVisible(
            pages_[pending_page]->GetPageFeatures());
        client_->Invalidate(GetPageScreenRect(pending_page));
      }
    }
  }
  pending_pages_.swap(still_pending);
  if (update_pages)
    LoadPageInfo(true);
}
