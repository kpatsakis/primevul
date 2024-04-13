bool Document::dispatchBeforeUnloadEvent(ChromeClient& chromeClient, bool isReload, bool& didAllowNavigation)
{
    if (!m_domWindow)
        return true;

    if (!body())
        return true;

    if (processingBeforeUnload())
        return false;

    RefPtrWillBeRawPtr<Document> protect(this);

    RefPtrWillBeRawPtr<BeforeUnloadEvent> beforeUnloadEvent = BeforeUnloadEvent::create();
    m_loadEventProgress = BeforeUnloadEventInProgress;
    m_domWindow->dispatchEvent(beforeUnloadEvent.get(), this);
    m_loadEventProgress = BeforeUnloadEventCompleted;
    if (!beforeUnloadEvent->defaultPrevented())
        defaultEventHandler(beforeUnloadEvent.get());
    if (!frame() || beforeUnloadEvent->returnValue().isNull())
        return true;

    if (didAllowNavigation) {
        addConsoleMessage(ConsoleMessage::create(JSMessageSource, ErrorMessageLevel, "Blocked attempt to show multiple 'beforeunload' confirmation panels for a single navigation."));
        return true;
    }

    String text = beforeUnloadEvent->returnValue();
    if (chromeClient.openBeforeUnloadConfirmPanel(text, m_frame, isReload)) {
        didAllowNavigation = true;
        return true;
    }
    return false;
}
