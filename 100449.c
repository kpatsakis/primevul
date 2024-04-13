void InlineTextBox::paintSelection(GraphicsContext* context, int tx, int ty, RenderStyle* style, const Font& font)
{
    int sPos, ePos;
    selectionStartEnd(sPos, ePos);
    if (sPos >= ePos)
        return;

    Color textColor = style->visitedDependentColor(CSSPropertyColor);
    Color c = renderer()->selectionBackgroundColor();
    if (!c.isValid() || c.alpha() == 0)
        return;

    if (textColor == c)
        c = Color(0xff - c.red(), 0xff - c.green(), 0xff - c.blue());

    context->save();
    updateGraphicsContext(context, c, c, 0, style->colorSpace());  // Don't draw text at all!
    int y = selectionTop();
    int h = selectionHeight();
    int length = m_truncation != cNoTruncation ? m_truncation : m_len;
    const UChar* characters = textRenderer()->text()->characters() + m_start;

    BufferForAppendingHyphen charactersWithHyphen;
    if (ePos == length && hasHyphen()) {
        adjustCharactersAndLengthForHyphen(charactersWithHyphen, style, characters, length);
        ePos = length;
    }

    context->clip(IntRect(m_x + tx, y + ty, m_logicalWidth, h));
    context->drawHighlightForText(font, TextRun(characters, length, textRenderer()->allowTabs(), textPos(), m_toAdd, 
                                  !isLeftToRightDirection(), m_dirOverride || style->visuallyOrdered()),
                                  IntPoint(m_x + tx, y + ty), h, c, style->colorSpace(), sPos, ePos);
    context->restore();
}
