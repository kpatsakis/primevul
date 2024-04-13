void LayerTilerChromium::reset()
{
    m_tiles.clear();
    m_unusedTiles.clear();

    m_layerSize = IntSize();
    m_layerTileSize = IntSize();
    m_lastUpdateLayerRect = IntRect();
}
