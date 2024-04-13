bool PDFiumEngine::OnLeftMouseDown(const pp::MouseInputEvent& event) {
  SetMouseLeftButtonDown(true);

  auto selection_invalidator =
      std::make_unique<SelectionChangeInvalidator>(this);
  selection_.clear();

  int page_index = -1;
  int char_index = -1;
  int form_type = FPDF_FORMFIELD_UNKNOWN;
  PDFiumPage::LinkTarget target;
  pp::Point point = event.GetPosition();
  PDFiumPage::Area area =
      GetCharIndex(point, &page_index, &char_index, &form_type, &target);
  DCHECK_GE(form_type, FPDF_FORMFIELD_UNKNOWN);
  mouse_down_state_.Set(area, target);

  if (IsLinkArea(area))
    return true;

  if (page_index != -1) {
    last_page_mouse_down_ = page_index;
    double page_x;
    double page_y;
    DeviceToPage(page_index, point.x(), point.y(), &page_x, &page_y);

    bool is_form_text_area = IsFormTextArea(area, form_type);
    FPDF_PAGE page = pages_[page_index]->GetPage();
    bool is_editable_form_text_area =
        is_form_text_area &&
        IsPointInEditableFormTextArea(page, page_x, page_y, form_type);

    FORM_OnLButtonDown(form_, page, 0, page_x, page_y);
    if (form_type != FPDF_FORMFIELD_UNKNOWN) {
      selection_invalidator.reset();

      SetInFormTextArea(is_form_text_area);
      editable_form_text_area_ = is_editable_form_text_area;
      return true;  // Return now before we get into the selection code.
    }
  }
  SetInFormTextArea(false);

  if (area != PDFiumPage::TEXT_AREA)
    return true;  // Return true so WebKit doesn't do its own highlighting.

  if (event.GetClickCount() == 1)
    OnSingleClick(page_index, char_index);
  else if (event.GetClickCount() == 2 || event.GetClickCount() == 3)
    OnMultipleClick(event.GetClickCount(), page_index, char_index);

  return true;
}
