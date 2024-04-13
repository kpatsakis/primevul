void RenderLayerCompositor::clearMappingForAllRenderLayers()
{
    clearMappingForRenderLayerIncludingDescendants(m_renderView->layer());
}
