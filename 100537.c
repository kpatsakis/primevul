bool EditorClientBlackBerry::shouldBeginEditing(Range* range)
{
    if (m_webPagePrivate->m_dumpRenderTree)
        return m_webPagePrivate->m_dumpRenderTree->shouldBeginEditingInDOMRange(range);

    return true;
}
