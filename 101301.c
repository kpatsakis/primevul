IntSize PaintLayerScrollableArea::MaximumScrollOffsetInt() const {
  if (!GetLayoutBox() || !GetLayoutBox()->HasOverflowClip())
    return ToIntSize(-ScrollOrigin());

  IntSize content_size = ContentsSize();

  Page* page = GetLayoutBox()->GetDocument().GetPage();
  DCHECK(page);
  TopDocumentRootScrollerController& controller =
      page->GlobalRootScrollerController();

  IntSize visible_size;
  if (this == controller.RootScrollerArea()) {
    visible_size = controller.RootScrollerVisibleArea();
  } else {
    visible_size =
        PixelSnappedIntRect(GetLayoutBox()->OverflowClipRect(
                                GetLayoutBox()->Location(),
                                kIgnorePlatformAndCSSOverlayScrollbarSize))
            .Size();
  }

  content_size = content_size.ExpandedTo(visible_size);

  return ToIntSize(-ScrollOrigin() + (content_size - visible_size));
}
