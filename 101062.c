ResourceFetcher::~ResourceFetcher()
{
    m_documentLoader = 0;
    m_document = nullptr;

    clearPreloads();

    ASSERT(!m_requestCount);
}
