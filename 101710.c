void PDFiumEngine::OnMultipleClick(int click_count,
                                   int page_index,
                                   int char_index) {
  DCHECK_GE(click_count, 2);
  bool is_double_click = click_count == 2;

  int start_index = char_index;
  do {
    base::char16 cur = pages_[page_index]->GetCharAtIndex(start_index);
    if (FindMultipleClickBoundary(is_double_click, cur))
      break;
  } while (--start_index >= 0);
  if (start_index)
    start_index++;

  int end_index = char_index;
  int total = pages_[page_index]->GetCharCount();
  while (end_index++ <= total) {
    base::char16 cur = pages_[page_index]->GetCharAtIndex(end_index);
    if (FindMultipleClickBoundary(is_double_click, cur))
      break;
  }

  selection_.push_back(PDFiumRange(pages_[page_index].get(), start_index,
                                   end_index - start_index));
}
