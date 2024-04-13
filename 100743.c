void FrameLoader::clear(ClearOptions options)
{
    if (m_stateMachine.creatingInitialEmptyDocument())
        return;

    m_frame->editor().clear();
    m_frame->document()->cancelParsing();
    m_frame->document()->stopActiveDOMObjects();
    if (m_frame->document()->attached()) {
        m_frame->document()->prepareForDestruction();
        m_frame->document()->removeFocusedElementOfSubtree(m_frame->document());
    }

    if (options & ClearWindowProperties) {
        InspectorInstrumentation::frameWindowDiscarded(m_frame, m_frame->domWindow());
        m_frame->domWindow()->reset();
        m_frame->script()->clearWindowShell();
    }

    m_frame->selection().prepareForDestruction();
    m_frame->eventHandler()->clear();
    if (m_frame->view())
        m_frame->view()->clear();

    if (options & ClearWindowObject) {
        m_frame->setDOMWindow(0);
    }

    m_containsPlugins = false;

    if (options & ClearScriptObjects)
        m_frame->script()->clearScriptObjects();

    m_frame->script()->enableEval();

    m_frame->navigationScheduler()->clear();

    m_checkTimer.stop();
    m_shouldCallCheckCompleted = false;

    if (m_stateMachine.isDisplayingInitialEmptyDocument())
        m_stateMachine.advanceTo(FrameLoaderStateMachine::CommittedFirstRealLoad);
}
