void DocumentThreadableLoader::didTimeout(Timer<DocumentThreadableLoader>* timer)
{
    ASSERT_UNUSED(timer, timer == &m_timeoutTimer);

    static const int timeoutError = -7;
    ResourceError error("net", timeoutError, resource()->url(), String());
    error.setIsTimeout(true);
    cancelWithError(error);
}
