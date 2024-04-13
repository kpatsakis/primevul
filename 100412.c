void RenderBox::willBeDestroyed()
{
    clearOverrideSize();

    if (style() && (style()->logicalHeight().isPercent() || style()->logicalMinHeight().isPercent() || style()->logicalMaxHeight().isPercent()))
        RenderBlock::removePercentHeightDescendant(this);

    ASSERT(!RenderBlock::hasPercentHeightDescendant(this));

    RenderBoxModelObject::willBeDestroyed();
}
