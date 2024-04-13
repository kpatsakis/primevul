bool InlineTextBox::nodeAtPoint(const HitTestRequest&, HitTestResult& result, int x, int y, int tx, int ty)
{
    if (isLineBreak())
        return false;

    IntRect rect(tx + m_x, ty + m_y, width(), height());
    if (m_truncation != cFullTruncation && visibleToHitTesting() && rect.intersects(result.rectForPoint(x, y))) {
        renderer()->updateHitTestResult(result, IntPoint(x - tx, y - ty));
        if (!result.addNodeToRectBasedTestResult(renderer()->node(), x, y, rect))
            return true;
    }
    return false;
}
