void FrameLoader::checkNewWindowPolicyAndContinue(PassRefPtr<FormState> formState, const String& frameName, const NavigationAction& action)
{
    if (m_frame->document()->pageDismissalEventBeingDispatched() != Document::NoDismissal)
        return;

    if (m_frame->document() && m_frame->document()->isSandboxed(SandboxPopups))
        return;

    if (!DOMWindow::allowPopUp(m_frame))
        return;

    NavigationPolicy navigationPolicy = NavigationPolicyNewForegroundTab;
    action.specifiesNavigationPolicy(&navigationPolicy);

    if (navigationPolicy == NavigationPolicyDownload) {
        m_client->loadURLExternally(action.resourceRequest(), navigationPolicy);
        return;
    }

    RefPtr<Frame> frame = m_frame;
    RefPtr<Frame> mainFrame = m_frame;

    if (!m_frame->settings() || m_frame->settings()->supportsMultipleWindows()) {
        struct WindowFeatures features;
        Page* newPage = m_frame->page()->chrome().client().createWindow(m_frame, FrameLoadRequest(m_frame->document()->securityOrigin(), action.resourceRequest()),
            features, navigationPolicy);

        if (!newPage)
            return;
        mainFrame = newPage->mainFrame();
    }

    if (frameName != "_blank")
        mainFrame->tree()->setName(frameName);

    mainFrame->page()->setOpenedByDOM();
    mainFrame->page()->chrome().show(navigationPolicy);
    if (!m_suppressOpenerInNewFrame) {
        mainFrame->loader()->setOpener(frame.get());
        mainFrame->document()->setReferrerPolicy(frame->document()->referrerPolicy());
    }

    mainFrame->loader()->loadWithNavigationAction(action.resourceRequest(), NavigationAction(action.resourceRequest()), FrameLoadTypeStandard, formState, SubstituteData());
}
