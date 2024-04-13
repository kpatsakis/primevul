int PDFiumEngine::GetCharCount(int page_index) {
  DCHECK(PageIndexInBounds(page_index));
  return pages_[page_index]->GetCharCount();
}
