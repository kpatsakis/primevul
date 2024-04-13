bool PaintLayerScrollableArea::NeedsScrollbarReconstruction() const {
  if (!HasScrollbar())
    return false;

  const LayoutObject& style_source = ScrollbarStyleSource(*GetLayoutBox());
  bool needs_custom =
      style_source.IsBox() &&
      style_source.StyleRef().HasPseudoStyle(kPseudoIdScrollbar);

  Scrollbar* scrollbars[] = {HorizontalScrollbar(), VerticalScrollbar()};

  for (Scrollbar* scrollbar : scrollbars) {
    if (!scrollbar)
      continue;

    if (scrollbar->IsCustomScrollbar() != needs_custom)
      return true;

    if (needs_custom) {
      DCHECK(scrollbar->IsCustomScrollbar());
      if (ToLayoutScrollbar(scrollbar)->StyleSource()->GetLayoutObject() !=
          style_source) {
        return true;
      }

      continue;
    }

    Page* page = GetLayoutBox()->GetFrame()->LocalFrameRoot().GetPage();
    DCHECK(page);
    ScrollbarTheme* current_theme = &page->GetScrollbarTheme();

    if (current_theme != &scrollbar->GetTheme())
      return true;
  }
  return false;
}
