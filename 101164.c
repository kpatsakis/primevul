void Document::enqueueUniqueAnimationFrameEvent(PassRefPtrWillBeRawPtr<Event> event)
{
    ensureScriptedAnimationController().enqueuePerFrameEvent(event);
}
