void OmniboxViewWin::OnPaint(HDC bogus_hdc) {

  CRect paint_clip_rect;
  if (!GetUpdateRect(&paint_clip_rect, true))
    return;

  CPaintDC paint_dc(m_hWnd);
  CDC memory_dc(CreateCompatibleDC(paint_dc));
  CRect rect;
  GetClientRect(&rect);
  CBitmap memory_bitmap(CreateCompatibleBitmap(paint_dc, rect.Width(),
                                               rect.Height()));
  HBITMAP old_bitmap = memory_dc.SelectBitmap(memory_bitmap);

  HWND old_edit_hwnd = edit_hwnd;
  edit_hwnd = m_hWnd;
  paint_struct = paint_dc.m_ps;
  paint_struct.hdc = memory_dc;
  DefWindowProc(WM_PAINT, reinterpret_cast<WPARAM>(bogus_hdc), 0);

  EraseTopOfSelection(&memory_dc, rect, paint_clip_rect);

  if (insecure_scheme_component_.is_nonempty())
    DrawSlashForInsecureScheme(memory_dc, rect, paint_clip_rect);

  if (drop_highlight_position_ != -1)
    DrawDropHighlight(memory_dc, rect, paint_clip_rect);

  BitBlt(paint_dc, rect.left, rect.top, rect.Width(), rect.Height(), memory_dc,
         rect.left, rect.top, SRCCOPY);
  memory_dc.SelectBitmap(old_bitmap);
  edit_hwnd = old_edit_hwnd;
}
