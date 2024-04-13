IntRect LayerTilerChromium::tileContentRect(int i, int j) const
{
    IntPoint anchor(m_layerPosition.x() + i * m_tileSize.width(), m_layerPosition.y() + j * m_tileSize.height());
    IntRect tile(anchor, m_tileSize);
    return tile;
}
