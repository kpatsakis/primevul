void FrameView::sendResizeEventIfNeeded()
{
    ASSERT(m_frame);

    RenderView* renderView = this->renderView();
    if (!renderView || renderView->document().printing())
        return;

    IntSize currentSize = layoutSize(IncludeScrollbars);
    float currentZoomFactor = renderView->style()->zoom();

    bool shouldSendResizeEvent = currentSize != m_lastViewportSize || currentZoomFactor != m_lastZoomFactor;

    m_lastViewportSize = currentSize;
    m_lastZoomFactor = currentZoomFactor;

    if (!shouldSendResizeEvent)
        return;

    m_frame->document()->enqueueResizeEvent();

    if (m_frame->isMainFrame())
        InspectorInstrumentation::didResizeMainFrame(m_frame->page());
}
