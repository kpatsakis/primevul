ResourcePtr<ImageResource> ResourceFetcher::fetchImage(FetchRequest& request)
{
    if (LocalFrame* f = frame()) {
        if (f->document()->pageDismissalEventBeingDispatched() != Document::NoDismissal) {
            KURL requestURL = request.resourceRequest().url();
            if (requestURL.isValid() && canRequest(Resource::Image, requestURL, request.options(), request.forPreload(), request.originRestriction()))
                PingLoader::loadImage(f, requestURL);
            return 0;
        }
    }

    if (request.resourceRequest().url().protocolIsData())
        preCacheDataURIImage(request);

    request.setDefer(clientDefersImage(request.resourceRequest().url()) ? FetchRequest::DeferredByClient : FetchRequest::NoDefer);
    return toImageResource(requestResource(Resource::Image, request));
}
