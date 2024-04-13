GraphicsLayer* RenderLayerScrollableArea::layerForHorizontalScrollbar() const
{
    DisableCompositingQueryAsserts disabler;

    return layer()->hasCompositedLayerMapping() ? layer()->compositedLayerMapping()->layerForHorizontalScrollbar() : 0;
}
