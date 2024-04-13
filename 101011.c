void FrameView::updateCompositedSelectionBoundsIfNeeded()
{
    if (!RuntimeEnabledFeatures::compositedSelectionUpdatesEnabled())
        return;

    Page* page = frame().page();
    ASSERT(page);

    LocalFrame* frame = toLocalFrame(page->focusController().focusedOrMainFrame());
    if (!frame || !frame->selection().isCaretOrRange()) {
        page->chrome().client().clearCompositedSelectionBounds();
        return;
    }

}
