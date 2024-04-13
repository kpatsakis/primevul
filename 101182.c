void Document::updateLayout()
{
    ASSERT(isMainThread());

    ScriptForbiddenScope forbidScript;

    RefPtrWillBeRawPtr<FrameView> frameView = view();
    if (frameView && frameView->isInPerformLayout()) {
        ASSERT_NOT_REACHED();
        return;
    }

    if (HTMLFrameOwnerElement* owner = ownerElement())
        owner->document().updateLayout();

    updateLayoutTreeIfNeeded();

    if (!isActive())
        return;

    if (frameView->needsLayout())
        frameView->layout();

    if (lifecycle().state() < DocumentLifecycle::LayoutClean)
        lifecycle().advanceTo(DocumentLifecycle::LayoutClean);
}
