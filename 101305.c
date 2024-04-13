static LayoutRect ScrollControlVisualRect(
    const IntRect& scroll_control_rect,
    const LayoutBox& box,
    const PaintInvalidatorContext& context,
    const LayoutRect& previous_visual_rect) {
  LayoutRect visual_rect(scroll_control_rect);
#if DCHECK_IS_ON()
  FindVisualRectNeedingUpdateScope finder(box, context, previous_visual_rect,
                                          visual_rect);
#endif
  if (!context.NeedsVisualRectUpdate(box))
    return previous_visual_rect;

  return visual_rect;
}
