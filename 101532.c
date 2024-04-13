void PDFiumEngine::LoadPageInfo(bool reload) {
  pending_pages_.clear();
  pp::Size old_document_size = document_size_;
  document_size_ = pp::Size();
  std::vector<pp::Rect> page_rects;
  int page_count = FPDF_GetPageCount(doc_);
  bool doc_complete = doc_loader_.IsDocumentComplete();
  bool is_linear = FPDFAvail_IsLinearized(fpdf_availability_) == PDF_LINEARIZED;
  for (int i = 0; i < page_count; ++i) {
    if (i != 0) {
      document_size_.Enlarge(0, kPageSeparatorThickness);
    }

    bool page_available;
    if (reload) {
      page_available = pages_[i]->available();
    } else if (is_linear) {
      int linear_page_avail =
          FPDFAvail_IsPageAvail(fpdf_availability_, i, &download_hints_);
      page_available = linear_page_avail == PDF_DATA_AVAIL;
    } else {
      page_available = doc_complete;
    }

    pp::Size size = page_available ? GetPageSize(i) : default_page_size_;
    size.Enlarge(kPageShadowLeft + kPageShadowRight,
                 kPageShadowTop + kPageShadowBottom);
    pp::Rect rect(pp::Point(0, document_size_.height()), size);
    page_rects.push_back(rect);

    if (size.width() > document_size_.width())
      document_size_.set_width(size.width());

    document_size_.Enlarge(0, size.height());
  }

  for (int i = 0; i < page_count; ++i) {
    page_rects[i].set_x((document_size_.width() - page_rects[i].width()) / 2);
    pp::Rect page_rect(page_rects[i]);
    page_rect.Inset(kPageShadowLeft, kPageShadowTop,
                    kPageShadowRight, kPageShadowBottom);
    if (reload) {
      pages_[i]->set_rect(page_rect);
    } else {
      pages_.push_back(new PDFiumPage(this, i, page_rect, false));
    }
  }

  CalculateVisiblePages();
  if (document_size_ != old_document_size)
    client_->DocumentSizeUpdated(document_size_);
}
