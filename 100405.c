IntRect RenderBox::localCaretRect(InlineBox* box, int caretOffset, int* extraWidthToEndOfLine)
{

    IntRect rect(x(), y(), caretWidth, height());
    bool ltr = box ? box->isLeftToRightDirection() : style()->isLeftToRightDirection();

    if ((!caretOffset) ^ ltr)
        rect.move(IntSize(width() - caretWidth, 0));

    if (box) {
        RootInlineBox* rootBox = box->root();
        int top = rootBox->lineTop();
        rect.setY(top);
        rect.setHeight(rootBox->lineBottom() - top);
    }

    int fontHeight = style()->fontMetrics().height();
    if (fontHeight > rect.height() || (!isReplaced() && !isTable()))
        rect.setHeight(fontHeight);

    if (extraWidthToEndOfLine)
        *extraWidthToEndOfLine = x() + width() - rect.maxX();

    rect.moveBy(-location());
    return rect;
}
