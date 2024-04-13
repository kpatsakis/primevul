LONG OmniboxViewWin::ClipXCoordToVisibleText(LONG x,
                                             bool is_triple_click) const {
  PARAFORMAT2 pf2;
  GetParaFormat(pf2);
  const bool ltr_text_in_ltr_layout = !(pf2.wEffects & PFE_RTLPARA) &&
      !base::i18n::StringContainsStrongRTLChars(GetText());
  const int length = GetTextLength();
  RECT r;
  GetRect(&r);
  const LONG first_position_x = PosFromChar(0).x;
  LONG min_x = first_position_x;
  if (!ltr_text_in_ltr_layout) {
    for (int i = 1; i < length; ++i)
      min_x = std::min(min_x, PosFromChar(i).x);
  }
  const LONG left_bound = std::max(r.left, min_x);
  LONG right_bound = r.right;
  LONG end_position_x = PosFromChar(length).x;
  if (end_position_x >= first_position_x) {
    right_bound = std::min(right_bound, end_position_x);  // LTR case.
  }
  if (x < left_bound) {
    return (is_triple_click && ltr_text_in_ltr_layout) ? left_bound - 1 :
                                                         left_bound;
  }
  if ((length == 0) || (x < right_bound))
    return x;
  return is_triple_click ? (right_bound - 1) : right_bound;
}
