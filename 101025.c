int Instance::GetPageNumberToDisplay() {
  int page = engine_->GetMostVisiblePage();
  if (IsPrintPreview() && !print_preview_page_numbers_.empty()) {
    page = ClipToRange<int>(page, 0, print_preview_page_numbers_.size() - 1);
    return print_preview_page_numbers_[page];
  }
  return page + 1;
}
