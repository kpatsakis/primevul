bool PDFiumEngine::CheckPageAvailable(int index, std::vector<int>* pending) {
  if (!doc_)
    return false;

  const int num_pages = static_cast<int>(pages_.size());
  if (index < num_pages && pages_[index]->available())
    return true;

  if (!FPDFAvail_IsPageAvail(fpdf_availability_, index, &download_hints_)) {
    if (!base::ContainsValue(*pending, index))
      pending->push_back(index);
    return false;
  }

  if (index < num_pages)
    pages_[index]->set_available(true);
  if (default_page_size_.IsEmpty())
    default_page_size_ = GetPageSize(index);
  return true;
}
