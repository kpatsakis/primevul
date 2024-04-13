InlineBoxPosition AdjustInlineBoxPositionForTextDirection(
    InlineBox* inline_box,
    int caret_offset,
    UnicodeBidi unicode_bidi,
    TextDirection primary_direction) {
  if (inline_box->Direction() == primary_direction)
    return AdjustInlineBoxPositionForPrimaryDirection(inline_box, caret_offset);

  const unsigned char level = inline_box->BidiLevel();
  if (caret_offset == inline_box->CaretLeftmostOffset()) {
    InlineBox* const prev_box = inline_box->PrevLeafChildIgnoringLineBreak();
    if (!prev_box || prev_box->BidiLevel() < level) {
      InlineBox* const result_box =
          InlineBoxTraversal::FindRightBoundaryOfEntireBidiRunIgnoringLineBreak(
              *inline_box, level);
      return InlineBoxPosition(result_box, result_box->CaretRightmostOffset());
    }

    if (prev_box->BidiLevel() <= level)
      return InlineBoxPosition(inline_box, caret_offset);
    InlineBox* const result_box =
        InlineBoxTraversal::FindLeftBoundaryOfBidiRunIgnoringLineBreak(
            *inline_box, level);
    return InlineBoxPosition(result_box, result_box->CaretLeftmostOffset());
  }

  if (unicode_bidi == UnicodeBidi::kPlaintext) {
    if (inline_box->BidiLevel() < level)
      return InlineBoxPosition(inline_box, inline_box->CaretLeftmostOffset());
    return InlineBoxPosition(inline_box, inline_box->CaretRightmostOffset());
  }

  InlineBox* const next_box = inline_box->NextLeafChildIgnoringLineBreak();
  if (!next_box || next_box->BidiLevel() < level) {
    InlineBox* const result_box =
        InlineBoxTraversal::FindLeftBoundaryOfEntireBidiRunIgnoringLineBreak(
            *inline_box, level);
    return InlineBoxPosition(result_box, result_box->CaretLeftmostOffset());
  }

  if (next_box->BidiLevel() <= level)
    return InlineBoxPosition(inline_box, caret_offset);

  InlineBox* const result_box =
      InlineBoxTraversal::FindRightBoundaryOfBidiRunIgnoringLineBreak(
          *inline_box, level);
  return InlineBoxPosition(result_box, result_box->CaretRightmostOffset());
}
