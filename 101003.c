RenderBox* FrameView::embeddedContentBox() const
{
    RenderView* renderView = this->renderView();
    if (!renderView)
        return 0;

    RenderObject* firstChild = renderView->firstChild();
    if (!firstChild || !firstChild->isBox())
        return 0;

    if (firstChild->isSVGRoot())
        return toRenderBox(firstChild);

    return 0;
}
