void Document::tasksWereResumed()
{
    scriptRunner()->resume();

    if (m_parser)
        m_parser->resumeScheduledTasks();
    if (m_scriptedAnimationController)
        m_scriptedAnimationController->resume();

    MutationObserver::resumeSuspendedObservers();
    if (m_domWindow)
        DOMWindowPerformance::performance(*m_domWindow)->resumeSuspendedObservers();
}
