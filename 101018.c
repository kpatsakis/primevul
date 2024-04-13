GraphicsLayer* RenderLayerScrollableArea::layerForVerticalScrollbar() const
{
    DisableCompositingQueryAsserts disabler;

    return layer()->hasCompositedLayerMapping() ? layer()->compositedLayerMapping()->layerForVerticalScrollbar() : 0;
}
