Frame* FrameLoader::loadSubframe(HTMLFrameOwnerElement* ownerElement, const KURL& url, const String& name, const String& referrer)
{
    bool allowsScrolling = true;
    int marginWidth = -1;
    int marginHeight = -1;
    if (ownerElement->hasTagName(frameTag) || ownerElement->hasTagName(iframeTag)) {
        HTMLFrameElementBase* o = static_cast<HTMLFrameElementBase*>(ownerElement);
        allowsScrolling = o->scrollingMode() != ScrollbarAlwaysOff;
        marginWidth = o->getMarginWidth();
        marginHeight = o->getMarginHeight();
    }

    if (!SecurityOrigin::canLoad(url, referrer, 0)) {
        FrameLoader::reportLocalLoadFailed(m_frame, url.string());
        return 0;
    }

    bool hideReferrer = SecurityOrigin::shouldHideReferrer(url, referrer);
    RefPtr<Frame> frame = m_client->createFrame(url, name, ownerElement, hideReferrer ? String() : referrer, allowsScrolling, marginWidth, marginHeight);

    if (!frame)  {
        checkCallImplicitClose();
        return 0;
    }
    
    frame->loader()->m_isComplete = false;
   
    RenderObject* renderer = ownerElement->renderer();
    FrameView* view = frame->view();
    if (renderer && renderer->isWidget() && view)
        toRenderWidget(renderer)->setWidget(view);
    
    checkCallImplicitClose();
    
    if (url.isEmpty() || url == blankURL()) {
        frame->loader()->completed();
        frame->loader()->checkCompleted();
    }

    return frame.get();
}
