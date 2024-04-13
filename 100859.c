CompositingReasons RenderLayerCompositor::directReasonsForCompositing(const RenderLayer* layer) const
{
    RenderObject* renderer = layer->renderer();
    CompositingReasons directReasons = CompositingReasonNone;

    if (requiresCompositingForTransform(renderer))
        directReasons |= CompositingReason3DTransform;

    if (requiresCompositingForVideo(renderer))
        directReasons |= CompositingReasonVideo;
    else if (requiresCompositingForCanvas(renderer))
        directReasons |= CompositingReasonCanvas;
    else if (requiresCompositingForPlugin(renderer))
        directReasons |= CompositingReasonPlugin;
    else if (requiresCompositingForFrame(renderer))
        directReasons |= CompositingReasonIFrame;

    if (requiresCompositingForBackfaceVisibilityHidden(renderer))
        directReasons |= CompositingReasonBackfaceVisibilityHidden;

    if (requiresCompositingForAnimation(renderer))
        directReasons |= CompositingReasonAnimation;

    if (requiresCompositingForTransition(renderer))
        directReasons |= CompositingReasonAnimation;

    if (requiresCompositingForFilters(renderer))
        directReasons |= CompositingReasonFilters;

    if (requiresCompositingForPosition(renderer, layer))
        directReasons |= renderer->style()->position() == FixedPosition ? CompositingReasonPositionFixed : CompositingReasonPositionSticky;

    if (requiresCompositingForOverflowScrolling(layer))
        directReasons |= CompositingReasonOverflowScrollingTouch;

    if (requiresCompositingForOverflowScrollingParent(layer))
        directReasons |= CompositingReasonOverflowScrollingParent;

    if (requiresCompositingForOutOfFlowClipping(layer))
        directReasons |= CompositingReasonOutOfFlowClipping;

    return directReasons;
}
