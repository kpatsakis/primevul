void InlineTextBox::selectionStartEnd(int& sPos, int& ePos)
{
    int startPos, endPos;
    if (renderer()->selectionState() == RenderObject::SelectionInside) {
        startPos = 0;
        endPos = textRenderer()->textLength();
    } else {
        textRenderer()->selectionStartEnd(startPos, endPos);
        if (renderer()->selectionState() == RenderObject::SelectionStart)
            endPos = textRenderer()->textLength();
        else if (renderer()->selectionState() == RenderObject::SelectionEnd)
            startPos = 0;
    }

    sPos = max(startPos - m_start, 0);
    ePos = min(endPos - m_start, (int)m_len);
}
