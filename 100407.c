LayoutUnit RenderBox::marginEnd() const
{
    if (isHorizontalWritingMode())
        return style()->isLeftToRightDirection() ? m_marginRight : m_marginLeft;
    return style()->isLeftToRightDirection() ? m_marginBottom : m_marginTop;
}
