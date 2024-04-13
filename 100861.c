bool RenderLayerCompositor::layerHas3DContent(const RenderLayer* layer) const
{
    const RenderStyle* style = layer->renderer()->style();
    RenderLayerStackingNode* stackingNode = const_cast<RenderLayer*>(layer)->stackingNode();

    if (style &&
        (style->transformStyle3D() == TransformStyle3DPreserve3D ||
         style->hasPerspective() ||
         style->transform().has3DOperation()))
        return true;

    stackingNode->updateLayerListsIfNeeded();

#if !ASSERT_DISABLED
    LayerListMutationDetector mutationChecker(stackingNode);
#endif

    RenderLayerStackingNodeIterator iterator(*layer->stackingNode(), AllChildren);
    while (RenderLayerStackingNode* curNode = iterator.next()) {
        if (layerHas3DContent(curNode->layer()))
            return true;
    }

    return false;
}
