void Document::cancelIdleCallback(int id)
{
    if (!m_scriptedIdleTaskController)
        return;
    m_scriptedIdleTaskController->cancelCallback(id);
}
