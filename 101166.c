void Document::finishedParsing()
{
    ASSERT(!scriptableDocumentParser() || !m_parser->isParsing());
    ASSERT(!scriptableDocumentParser() || m_readyState != Loading);
    setParsingState(InDOMContentLoaded);

    if (!m_documentTiming.domContentLoadedEventStart())
        m_documentTiming.markDomContentLoadedEventStart();
    dispatchEvent(Event::createBubble(EventTypeNames::DOMContentLoaded));
    if (!m_documentTiming.domContentLoadedEventEnd())
        m_documentTiming.markDomContentLoadedEventEnd();
    setParsingState(FinishedParsing);

    RefPtrWillBeRawPtr<Document> protect(this);

    Microtask::performCheckpoint(V8PerIsolateData::mainThreadIsolate());

    if (RefPtrWillBeRawPtr<LocalFrame> frame = this->frame()) {
        const bool mainResourceWasAlreadyRequested = frame->loader().stateMachine()->committedFirstRealDocumentLoad();

        if (mainResourceWasAlreadyRequested)
            updateLayoutTreeIfNeeded();

        frame->loader().finishedParsing();

        TRACE_EVENT_INSTANT1("devtools.timeline", "MarkDOMContent", TRACE_EVENT_SCOPE_THREAD, "data", InspectorMarkLoadEvent::data(frame.get()));
        InspectorInstrumentation::domContentLoadedEventFired(frame.get());
    }

    m_elementDataCacheClearTimer.startOneShot(10, BLINK_FROM_HERE);

    m_fetcher->clearPreloads(ResourceFetcher::ClearSpeculativeMarkupPreloads);
}
