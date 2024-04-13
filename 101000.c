void DocumentThreadableLoader::start(const ResourceRequest& request)
{
    ASSERT(m_async || request.httpReferrer().isEmpty());

    m_sameOriginRequest = getSecurityOrigin()->canRequestNoSuborigin(request.url());
    m_requestContext = request.requestContext();
    m_redirectMode = request.fetchRedirectMode();

    if (!m_sameOriginRequest && m_options.crossOriginRequestPolicy == DenyCrossOriginRequests) {
        InspectorInstrumentation::documentThreadableLoaderFailedToStartLoadingForClient(m_document, m_client);
        ThreadableLoaderClient* client = m_client;
        clear();
        client->didFail(ResourceError(errorDomainBlinkInternal, 0, request.url().getString(), "Cross origin requests are not supported."));
        return;
    }

    m_requestStartedSeconds = monotonicallyIncreasingTime();

    const HTTPHeaderMap& headerMap = request.httpHeaderFields();
    for (const auto& header : headerMap) {
        if (FetchUtils::isSimpleHeader(header.key, header.value)) {
            m_simpleRequestHeaders.add(header.key, header.value);
        } else if (equalIgnoringCase(header.key, HTTPNames::Range) && m_options.crossOriginRequestPolicy == UseAccessControl && m_options.preflightPolicy == PreventPreflight) {
            m_simpleRequestHeaders.add(header.key, header.value);
        }
    }

    if (request.httpMethod() != HTTPNames::GET) {
        if (Page* page = m_document->page())
            page->chromeClient().didObserveNonGetFetchFromScript();
    }

    if (m_async && !request.skipServiceWorker() && SchemeRegistry::shouldTreatURLSchemeAsAllowingServiceWorkers(request.url().protocol()) && m_document->fetcher()->isControlledByServiceWorker()) {
        ResourceRequest newRequest(request);
        const WebURLRequest::RequestContext requestContext(request.requestContext());
        if (requestContext != WebURLRequest::RequestContextFetch) {
            switch (m_options.crossOriginRequestPolicy) {
            case DenyCrossOriginRequests:
                newRequest.setFetchRequestMode(WebURLRequest::FetchRequestModeSameOrigin);
                break;
            case UseAccessControl:
                if (m_options.preflightPolicy == ForcePreflight)
                    newRequest.setFetchRequestMode(WebURLRequest::FetchRequestModeCORSWithForcedPreflight);
                else
                    newRequest.setFetchRequestMode(WebURLRequest::FetchRequestModeCORS);
                break;
            case AllowCrossOriginRequests:
                SECURITY_CHECK(requestContext == WebURLRequest::RequestContextAudio || requestContext == WebURLRequest::RequestContextVideo || requestContext == WebURLRequest::RequestContextObject || requestContext == WebURLRequest::RequestContextFavicon || requestContext == WebURLRequest::RequestContextImage || requestContext == WebURLRequest::RequestContextScript);
                newRequest.setFetchRequestMode(WebURLRequest::FetchRequestModeNoCORS);
                break;
            }
            if (m_resourceLoaderOptions.allowCredentials == AllowStoredCredentials)
                newRequest.setFetchCredentialsMode(WebURLRequest::FetchCredentialsModeInclude);
            else
                newRequest.setFetchCredentialsMode(WebURLRequest::FetchCredentialsModeSameOrigin);
        }
        if (newRequest.fetchRequestMode() == WebURLRequest::FetchRequestModeCORS || newRequest.fetchRequestMode() == WebURLRequest::FetchRequestModeCORSWithForcedPreflight) {
            m_fallbackRequestForServiceWorker = ResourceRequest(request);
            m_fallbackRequestForServiceWorker.setSkipServiceWorker(true);
        }

        loadRequest(newRequest, m_resourceLoaderOptions);
        return;
    }

    dispatchInitialRequest(request);
}
