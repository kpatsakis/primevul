LayoutUnit RenderBox::availableLogicalHeight() const
{
    return availableLogicalHeightUsing(style()->logicalHeight());
}
