void Document::WillChangeFrameOwnerProperties(int margin_width,
                                              int margin_height,
                                              ScrollbarMode scrolling_mode,
                                              bool is_display_none) {
  DCHECK(GetFrame() && GetFrame()->Owner());
  FrameOwner* owner = GetFrame()->Owner();

  if (RuntimeEnabledFeatures::DisplayNoneIFrameCreatesNoLayoutObjectEnabled()) {
    if (documentElement()) {
      if (is_display_none != owner->IsDisplayNone())
        documentElement()->LazyReattachIfAttached();
    }
  }

  if (!body())
    return;

  if (margin_width != owner->MarginWidth())
    body()->SetIntegralAttribute(marginwidthAttr, margin_width);
  if (margin_height != owner->MarginHeight())
    body()->SetIntegralAttribute(marginheightAttr, margin_height);
  if (scrolling_mode != owner->ScrollingMode() && View())
    View()->SetNeedsLayout();
}
