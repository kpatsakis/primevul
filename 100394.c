void RenderBox::absoluteRects(Vector<LayoutRect>& rects, const LayoutPoint& accumulatedOffset)
{
    rects.append(LayoutRect(accumulatedOffset, size()));
}
