bool RenderLayerScrollableArea::hasVerticalOverflow() const
{
    ASSERT(!m_scrollDimensionsDirty);

    return pixelSnappedScrollHeight() > box().pixelSnappedClientHeight();
}
