bool PDFiumEngine::OnMouseMove(const pp::MouseInputEvent& event) {
  int page_index = -1;
  int char_index = -1;
  int form_type = FPDF_FORMFIELD_UNKNOWN;
  PDFiumPage::LinkTarget target;
  pp::Point point = event.GetPosition();
  PDFiumPage::Area area =
      GetCharIndex(point, &page_index, &char_index, &form_type, &target);

  if (!mouse_down_state_.Matches(area, target))
    mouse_down_state_.Reset();

  if (!selecting_) {
    PP_CursorType_Dev cursor;
    switch (area) {
      case PDFiumPage::TEXT_AREA:
        cursor = PP_CURSORTYPE_IBEAM;
        break;
      case PDFiumPage::WEBLINK_AREA:
      case PDFiumPage::DOCLINK_AREA:
        cursor = PP_CURSORTYPE_HAND;
        break;
      case PDFiumPage::NONSELECTABLE_AREA:
      case PDFiumPage::FORM_TEXT_AREA:
      default:
        switch (form_type) {
          case FPDF_FORMFIELD_PUSHBUTTON:
          case FPDF_FORMFIELD_CHECKBOX:
          case FPDF_FORMFIELD_RADIOBUTTON:
          case FPDF_FORMFIELD_COMBOBOX:
          case FPDF_FORMFIELD_LISTBOX:
            cursor = PP_CURSORTYPE_HAND;
            break;
          case FPDF_FORMFIELD_TEXTFIELD:
            cursor = PP_CURSORTYPE_IBEAM;
            break;
#if defined(PDF_ENABLE_XFA)
          case FPDF_FORMFIELD_XFA_CHECKBOX:
          case FPDF_FORMFIELD_XFA_COMBOBOX:
          case FPDF_FORMFIELD_XFA_IMAGEFIELD:
          case FPDF_FORMFIELD_XFA_LISTBOX:
          case FPDF_FORMFIELD_XFA_PUSHBUTTON:
          case FPDF_FORMFIELD_XFA_SIGNATURE:
            cursor = PP_CURSORTYPE_HAND;
            break;
          case FPDF_FORMFIELD_XFA_TEXTFIELD:
            cursor = PP_CURSORTYPE_IBEAM;
            break;
#endif
          default:
            cursor = PP_CURSORTYPE_POINTER;
            break;
        }
        break;
    }

    if (page_index != -1) {
      double page_x;
      double page_y;
      DeviceToPage(page_index, point.x(), point.y(), &page_x, &page_y);
      FORM_OnMouseMove(form_, pages_[page_index]->GetPage(), 0, page_x, page_y);
    }

    client_->UpdateCursor(cursor);
    std::string url = GetLinkAtPosition(event.GetPosition());
    if (url != link_under_cursor_) {
      link_under_cursor_ = url;
      pp::PDF::SetLinkUnderCursor(GetPluginInstance(), url.c_str());
    }

    if (mouse_left_button_down_ && area == PDFiumPage::FORM_TEXT_AREA &&
        last_page_mouse_down_ != -1) {
      SetFormSelectedText(form_, pages_[last_page_mouse_down_]->GetPage());
    }

    return false;
  }

  if (area != PDFiumPage::TEXT_AREA && !IsLinkArea(area))
    return false;

  SelectionChangeInvalidator selection_invalidator(this);
  return ExtendSelection(page_index, char_index);
}
