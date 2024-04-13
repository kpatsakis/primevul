void RenderLayerCompositor::updateGraphicsLayersMappedToRenderLayer(RenderLayer* layer)
{
    if (!layer->hasCompositedLayerMapping())
        return;

    CompositedLayerMappingPtr compositedLayerMapping = layer->compositedLayerMapping();

    compositedLayerMapping->updateCompositedBounds();

    if (layer->reflectionInfo()) {
        RenderLayer* reflectionLayer = layer->reflectionInfo()->reflectionLayer();
        ASSERT(reflectionLayer);
        if (reflectionLayer->hasCompositedLayerMapping())
            reflectionLayer->compositedLayerMapping()->updateCompositedBounds();
    }

    compositedLayerMapping->updateGraphicsLayerConfiguration();
    compositedLayerMapping->updateGraphicsLayerGeometry();

    if (!layer->parent())
        updateRootLayerPosition();

    if (compositedLayerMapping->hasUnpositionedOverflowControlsLayers())
        layer->scrollableArea()->positionOverflowControls();
}
