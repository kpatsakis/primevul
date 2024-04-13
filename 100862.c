bool RenderLayerCompositor::requiresCompositingForOutOfFlowClipping(const RenderLayer* layer) const
{
    return m_renderView->compositorDrivenAcceleratedScrollingEnabled() && layer->isUnclippedDescendant();
}
