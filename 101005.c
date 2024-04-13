void FrameView::gatherDebugLayoutRects(RenderObject* layoutRoot)
{
    bool isTracing;
    TRACE_EVENT_CATEGORY_GROUP_ENABLED(TRACE_DISABLED_BY_DEFAULT("blink.debug.layout"), &isTracing);
    if (!isTracing)
        return;
    if (!layoutRoot->enclosingLayer()->hasCompositedLayerMapping())
        return;
    DisableCompositingQueryAsserts disabler;
    GraphicsLayer* graphicsLayer = layoutRoot->enclosingLayer()->compositedLayerMapping()->mainGraphicsLayer();
    if (!graphicsLayer)
        return;

    GraphicsLayerDebugInfo& debugInfo = graphicsLayer->debugInfo();

    debugInfo.currentLayoutRects().clear();
    for (RenderObject* renderer = layoutRoot; renderer; renderer = renderer->nextInPreOrder()) {
        if (renderer->layoutDidGetCalled()) {
            FloatQuad quad = renderer->localToAbsoluteQuad(FloatQuad(renderer->previousPaintInvalidationRect()));
            LayoutRect rect = quad.enclosingBoundingBox();
            debugInfo.currentLayoutRects().append(rect);
            renderer->setLayoutDidGetCalled(false);
        }
    }
}
