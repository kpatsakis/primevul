PassRefPtrWillBeRawPtr<DocumentParser> Document::implicitOpen(ParserSynchronizationPolicy parserSyncPolicy)
{
    detachParser();

    removeChildren();
    ASSERT(!m_focusedElement);

    setCompatibilityMode(NoQuirksMode);

    if (!threadedParsingEnabledForTesting())
        parserSyncPolicy = ForceSynchronousParsing;

    m_parserSyncPolicy = parserSyncPolicy;
    m_parser = createParser();
    setParsingState(Parsing);
    setReadyState(Loading);

    return m_parser;
}
