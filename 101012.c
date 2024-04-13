IntRect FrameView::windowClipRect(IncludeScrollbarsInRect scrollbarInclusion) const
{
    ASSERT(m_frame->view() == this);

    if (paintsEntireContents())
        return IntRect(IntPoint(), contentsSize());

    IntRect clipRect = contentsToWindow(visibleContentRect(scrollbarInclusion));
    if (!m_frame->deprecatedLocalOwner())
        return clipRect;

    HTMLFrameOwnerElement* ownerElement = m_frame->deprecatedLocalOwner();
    FrameView* parentView = ownerElement->document().view();
    if (parentView)
        clipRect.intersect(parentView->windowClipRectForFrameOwner(ownerElement));
    return clipRect;
}
