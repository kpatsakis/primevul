int RenderBox::containingBlockLogicalWidthForContent() const
{
    RenderBlock* cb = containingBlock();
    if (shrinkToAvoidFloats())
        return cb->availableLogicalWidthForLine(y(), false);
    return cb->availableLogicalWidth();
}
