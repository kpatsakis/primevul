WeakPtrWillBeRawPtr<Document> Document::contextDocument()
{
    if (m_contextDocument)
        return m_contextDocument;
    if (m_frame) {
        return createWeakPtr();
    }
    return nullptr;
}
