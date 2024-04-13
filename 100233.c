void FrameLoader::loadFrameRequest(const FrameLoadRequest& request, bool lockHistory, bool lockBackForwardList,
    PassRefPtr<Event> event, PassRefPtr<FormState> formState, ReferrerPolicy referrerPolicy)
{    
    KURL url = request.resourceRequest().url();

    String referrer;
    String argsReferrer = request.resourceRequest().httpReferrer();
    if (!argsReferrer.isEmpty())
        referrer = argsReferrer;
    else
        referrer = m_outgoingReferrer;

    ASSERT(frame()->document());
    if (SecurityOrigin::shouldTreatURLAsLocal(url.string()) && !isFeedWithNestedProtocolInHTTPFamily(url)) {
        if (!SecurityOrigin::canLoad(url, String(), frame()->document()) && !SecurityOrigin::canLoad(url, referrer, 0)) {
            FrameLoader::reportLocalLoadFailed(m_frame, url.string());
            return;
        }
    }

    if (SecurityOrigin::shouldHideReferrer(url, referrer) || referrerPolicy == NoReferrer)
        referrer = String();
    
    FrameLoadType loadType;
    if (request.resourceRequest().cachePolicy() == ReloadIgnoringCacheData)
        loadType = FrameLoadTypeReload;
    else if (lockBackForwardList)
        loadType = FrameLoadTypeRedirectWithLockedBackForwardList;
    else
        loadType = FrameLoadTypeStandard;

    if (request.resourceRequest().httpMethod() == "POST")
        loadPostRequest(request.resourceRequest(), referrer, request.frameName(), lockHistory, loadType, event, formState.get());
    else
        loadURL(request.resourceRequest().url(), referrer, request.frameName(), lockHistory, loadType, event, formState.get());

    Frame* sourceFrame = formState ? formState->sourceFrame() : m_frame;
    Frame* targetFrame = sourceFrame->loader()->findFrameForNavigation(request.frameName());
    if (targetFrame && targetFrame != sourceFrame) {
        if (Page* page = targetFrame->page())
            page->chrome()->focus();
    }
}
