void OmniboxViewWin::OnMouseMove(UINT keys, const CPoint& point) {
  if (possible_drag_) {
    StartDragIfNecessary(point);
    return;
  }

  if (tracking_click_[kLeft] && !IsDrag(click_point_[kLeft], point))
    return;

  tracking_click_[kLeft] = false;

  if (!(keys & MK_LBUTTON)) {
    DefWindowProc(WM_MOUSEMOVE, keys, MAKELPARAM(point.x, point.y));
    return;
  }

  CHARRANGE selection;
  GetSel(selection);
  const bool possibly_can_discard_mousemove =
      (selection.cpMin == selection.cpMax) &&
      (((selection.cpMin == 0) &&
        (ClipXCoordToVisibleText(point.x, false) > point.x)) ||
       ((selection.cpMin == GetTextLength()) &&
        (ClipXCoordToVisibleText(point.x, false) < point.x)));
  if (!can_discard_mousemove_ || !possibly_can_discard_mousemove) {
    can_discard_mousemove_ = possibly_can_discard_mousemove;
    ScopedFreeze freeze(this, GetTextObjectModel());
    OnBeforePossibleChange();
    RECT r;
    GetRect(&r);
    DefWindowProc(WM_MOUSEMOVE, keys,
                  MAKELPARAM(point.x, (r.bottom - r.top) / 2));
    OnAfterPossibleChange();
  }
}
