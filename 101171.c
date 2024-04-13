void Document::pushCurrentScript(PassRefPtrWillBeRawPtr<HTMLScriptElement> newCurrentScript)
{
    ASSERT(newCurrentScript);
    m_currentScriptStack.append(newCurrentScript);
}
