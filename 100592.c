void OmniboxViewWin::EditDropTarget::UpdateDropHighlightPosition(
    const POINT& cursor_screen_position) {
  if (drag_has_string_) {
    POINT client_position = cursor_screen_position;
    ::ScreenToClient(edit_->m_hWnd, &client_position);
    int drop_position = edit_->CharFromPos(client_position);
    if (edit_->in_drag()) {
      LONG sel_start, sel_end;
      edit_->GetSel(sel_start, sel_end);
      if ((sel_start != sel_end) && (drop_position >= sel_start) &&
          (drop_position <= sel_end))
        drop_position = -1;
    } else {
      drop_position = -1;
    }
    edit_->SetDropHighlightPosition(drop_position);
  }
}
