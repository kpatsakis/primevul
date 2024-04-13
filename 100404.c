void RenderBox::flipForWritingMode(IntRect& rect) const
{
    if (!style()->isFlippedBlocksWritingMode())
        return;

    if (isHorizontalWritingMode())
        rect.setY(height() - rect.maxY());
    else
        rect.setX(width() - rect.maxX());
}
