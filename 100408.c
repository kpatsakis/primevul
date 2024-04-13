void RenderBox::paintCustomHighlight(const LayoutPoint& paintOffset, const AtomicString& type, bool behindText)
{
    Frame* frame = this->frame();
    if (!frame)
        return;
    Page* page = frame->page();
    if (!page)
        return;

    InlineBox* boxWrap = inlineBoxWrapper();
    RootInlineBox* r = boxWrap ? boxWrap->root() : 0;
    if (r) {
        FloatRect rootRect(paintOffset.x() + r->x(), paintOffset.y() + r->selectionTop(), r->logicalWidth(), r->selectionHeight());
        FloatRect imageRect(paintOffset.x() + x(), rootRect.y(), width(), rootRect.height());
        page->chrome()->client()->paintCustomHighlight(node(), type, imageRect, rootRect, behindText, false);
    } else {
        FloatRect imageRect(paintOffset.x() + x(), paintOffset.y() + y(), width(), height());
        page->chrome()->client()->paintCustomHighlight(node(), type, imageRect, imageRect, behindText, false);
    }
}
