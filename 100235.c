void FrameLoader::loadURL(const KURL& newURL, const String& referrer, const String& frameName, bool lockHistory, FrameLoadType newLoadType,
    PassRefPtr<Event> event, PassRefPtr<FormState> prpFormState)
{
    RefPtr<FormState> formState = prpFormState;
    bool isFormSubmission = formState;
    
    ResourceRequest request(newURL);
    if (!referrer.isEmpty()) {
        request.setHTTPReferrer(referrer);
        RefPtr<SecurityOrigin> referrerOrigin = SecurityOrigin::createFromString(referrer);
        addHTTPOriginIfNeeded(request, referrerOrigin->toString());
    }
    addExtraFieldsToRequest(request, newLoadType, true, event || isFormSubmission);
    if (newLoadType == FrameLoadTypeReload || newLoadType == FrameLoadTypeReloadFromOrigin)
        request.setCachePolicy(ReloadIgnoringCacheData);

    ASSERT(newLoadType != FrameLoadTypeSame);

    Frame* targetFrame = isFormSubmission ? 0 : findFrameForNavigation(frameName);
    if (targetFrame && targetFrame != m_frame) {
        targetFrame->loader()->loadURL(newURL, referrer, String(), lockHistory, newLoadType, event, formState.release());
        return;
    }

    if (m_unloadEventBeingDispatched)
        return;

    NavigationAction action(newURL, newLoadType, isFormSubmission, event);

    if (!targetFrame && !frameName.isEmpty()) {
        policyChecker()->checkNewWindowPolicy(action, FrameLoader::callContinueLoadAfterNewWindowPolicy,
            request, formState.release(), frameName, this);
        return;
    }

    RefPtr<DocumentLoader> oldDocumentLoader = m_documentLoader;

    bool sameURL = shouldTreatURLAsSameAsCurrent(newURL);
    
    if (shouldScrollToAnchor(isFormSubmission, newLoadType, newURL)) {
        oldDocumentLoader->setTriggeringAction(action);
        policyChecker()->stopCheck();
        policyChecker()->setLoadType(newLoadType);
        policyChecker()->checkNavigationPolicy(request, oldDocumentLoader.get(), formState.release(),
            callContinueFragmentScrollAfterNavigationPolicy, this);
    } else {
        bool isRedirect = m_quickRedirectComing;
        loadWithNavigationAction(request, action, lockHistory, newLoadType, formState.release());
        if (isRedirect) {
            m_quickRedirectComing = false;
            if (m_provisionalDocumentLoader)
                m_provisionalDocumentLoader->setIsClientRedirect(true);
        } else if (sameURL)
            m_loadType = FrameLoadTypeSame;
    }
}
