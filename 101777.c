void PDFiumEngine::GetSelection(uint32_t* selection_start_page_index,
                                uint32_t* selection_start_char_index,
                                uint32_t* selection_end_page_index,
                                uint32_t* selection_end_char_index) {
  size_t len = selection_.size();
  if (len == 0) {
    *selection_start_page_index = 0;
    *selection_start_char_index = 0;
    *selection_end_page_index = 0;
    *selection_end_char_index = 0;
    return;
  }

  *selection_start_page_index = selection_[0].page_index();
  *selection_start_char_index = selection_[0].char_index();
  *selection_end_page_index = selection_[len - 1].page_index();

  if (len == 1) {
    *selection_end_char_index =
        selection_[0].char_index() + selection_[0].char_count();
  } else {
    *selection_end_char_index = selection_[len - 1].char_count();
  }
}
