void PDFiumEngine::OnPendingRequestComplete() {
  if (!doc_ || !form_) {
    DCHECK(fpdf_availability_);
    LoadDocument();
    return;
  }

  bool update_pages = false;
  std::vector<int> still_pending;
  for (int pending_page : pending_pages_) {
    if (CheckPageAvailable(pending_page, &still_pending)) {
      update_pages = true;
      if (IsPageVisible(pending_page))
        client_->Invalidate(GetPageScreenRect(pending_page));
    }
  }
  pending_pages_.swap(still_pending);
  if (update_pages)
    LoadPageInfo(true);
}
