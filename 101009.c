bool FrameView::shouldSetCursor() const
{
    Page* page = frame().page();
    return page && page->visibilityState() != PageVisibilityStateHidden && page->focusController().isActive() && page->settings().deviceSupportsMouse();
}
