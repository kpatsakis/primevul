void OmniboxViewViews::SetCaretPos(size_t caret_pos) {
  SelectRange(gfx::Range(caret_pos, caret_pos));
}
