InlineBox* RenderBox::createInlineBox()
{
    return new (renderArena()) InlineBox(this);
}
