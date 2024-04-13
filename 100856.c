void RenderLayerCompositor::applyUpdateLayerCompositingStateChickenEggHacks(RenderLayer* layer, CompositingStateTransitionType compositedLayerUpdate)
{
    if (compositedLayerUpdate != NoCompositingStateChange)
        allocateOrClearCompositedLayerMapping(layer);
}
