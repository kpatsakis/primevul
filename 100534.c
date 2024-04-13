void EditorClientBlackBerry::didEndEditing()
{
    if (m_webPagePrivate->m_dumpRenderTree)
        m_webPagePrivate->m_dumpRenderTree->didEndEditing();
}
