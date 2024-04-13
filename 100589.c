void OmniboxViewWin::StartDragIfNecessary(const CPoint& point) {
  if (initiated_drag_ || !IsDrag(click_point_[kLeft], point))
    return;

  ui::OSExchangeData data;

  DWORD supported_modes = DROPEFFECT_COPY;

  CHARRANGE sel;
  GetSelection(sel);

  {
    ScopedFreeze freeze(this, GetTextObjectModel());
    DefWindowProc(WM_LBUTTONUP, 0,
                  MAKELPARAM(click_point_[kLeft].x, click_point_[kLeft].y));
    SetSelectionRange(sel);
  }

  const string16 start_text(GetText());
  string16 text_to_write(GetSelectedText());
  GURL url;
  bool write_url;
  const bool is_all_selected = IsSelectAllForRange(sel);

  model()->AdjustTextForCopy(std::min(sel.cpMin, sel.cpMax), is_all_selected,
                             &text_to_write, &url, &write_url);

  if (write_url) {
    string16 title;
    SkBitmap favicon;
    if (is_all_selected)
      model_->GetDataForURLExport(&url, &title, &favicon);
    drag_utils::SetURLAndDragImage(url, title, favicon, &data);
    supported_modes |= DROPEFFECT_LINK;
    content::RecordAction(UserMetricsAction("Omnibox_DragURL"));
  } else {
    supported_modes |= DROPEFFECT_MOVE;
    content::RecordAction(UserMetricsAction("Omnibox_DragString"));
  }

  data.SetString(text_to_write);

  scoped_refptr<ui::DragSource> drag_source(new ui::DragSource);
  DWORD dropped_mode;
  AutoReset<bool> auto_reset_in_drag(&in_drag_, true);
  if (DoDragDrop(ui::OSExchangeDataProviderWin::GetIDataObject(data),
                 drag_source, supported_modes, &dropped_mode) ==
          DRAGDROP_S_DROP) {
    if ((dropped_mode == DROPEFFECT_MOVE) && (start_text == GetText())) {
      ScopedFreeze freeze(this, GetTextObjectModel());
      OnBeforePossibleChange();
      SetSelectionRange(sel);
      ReplaceSel(L"", true);
      OnAfterPossibleChange();
    }
    possible_drag_ = false;
  } else {
    CPoint cursor_location;
    GetCursorPos(&cursor_location);

    CRect client_rect;
    GetClientRect(&client_rect);

    CPoint client_origin_on_screen(client_rect.left, client_rect.top);
    ClientToScreen(&client_origin_on_screen);
    client_rect.MoveToXY(client_origin_on_screen.x,
                         client_origin_on_screen.y);
    possible_drag_ = (client_rect.PtInRect(cursor_location) &&
                      ((GetKeyState(VK_LBUTTON) != 0) ||
                       (GetKeyState(VK_MBUTTON) != 0) ||
                       (GetKeyState(VK_RBUTTON) != 0)));
  }

  initiated_drag_ = true;
  tracking_click_[kLeft] = false;
}
