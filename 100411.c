void RenderBox::setScrollTop(int newTop)
{
    if (hasOverflowClip())
        layer()->scrollToYOffset(newTop, RenderLayer::ScrollOffsetClamped);
}
