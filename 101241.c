void OutOfProcessInstance::AppendBlankPrintPreviewPages() {
  if (print_preview_page_count_ == 0)
    return;
  engine_->AppendBlankPages(print_preview_page_count_);
  if (!preview_pages_info_.empty())
    LoadAvailablePreviewPage();
}
