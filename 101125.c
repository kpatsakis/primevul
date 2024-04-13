IntSize SVGImage::containerSize() const
{
    if (!m_page)
        return IntSize();
    LocalFrame* frame = m_page->mainFrame();
    SVGSVGElement* rootElement = toSVGDocument(frame->document())->rootElement();
    if (!rootElement)
        return IntSize();

    RenderSVGRoot* renderer = toRenderSVGRoot(rootElement->renderer());
    if (!renderer)
        return IntSize();

    IntSize containerSize = renderer->containerSize();
    if (!containerSize.isEmpty())
        return containerSize;

    ASSERT(renderer->style()->effectiveZoom() == 1);

    FloatSize currentSize;
    if (rootElement->intrinsicWidth().isFixed() && rootElement->intrinsicHeight().isFixed())
        currentSize = rootElement->currentViewportSize();
    else
        currentSize = rootElement->currentViewBoxRect().size();

    if (!currentSize.isEmpty())
        return IntSize(static_cast<int>(ceilf(currentSize.width())), static_cast<int>(ceilf(currentSize.height())));

    return IntSize(300, 150);
}
