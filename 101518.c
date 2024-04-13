void PDFiumEngine::FinishLoadingDocument() {
  DCHECK(doc_loader_.IsDocumentComplete() && doc_);

  bool need_update = false;
  for (size_t i = 0; i < pages_.size(); ++i) {
    if (pages_[i]->available())
      continue;

    pages_[i]->set_available(true);
    FPDFAvail_IsPageAvail(fpdf_availability_, i, &download_hints_);
    need_update = true;
    if (IsPageVisible(i))
      client_->Invalidate(GetPageScreenRect(i));
  }
  if (need_update)
    LoadPageInfo(true);

  if (called_do_document_action_)
    return;
  called_do_document_action_ = true;

  FORM_DoDocumentJSAction(form_);
  FORM_DoDocumentOpenAction(form_);
  if (most_visible_page_ != -1) {
    FPDF_PAGE new_page = pages_[most_visible_page_]->GetPage();
    FORM_DoPageAAction(new_page, form_, FPDFPAGE_AACTION_OPEN);
  }

  if (doc_)  // This can only happen if loading |doc_| fails.
    client_->DocumentLoadComplete(pages_.size());
}
