    ResourcePtr<Resource> resourceFromResourceRequest(ResourceRequest request, Resource::Type type = Resource::Raw)
    {
        if (request.url().isNull())
            request.setURL(KURL(ParsedURLString, kResourceURL));
        ResourcePtr<Resource> resource =
            new Resource(request, type);
        resource->setResponse(ResourceResponse(KURL(ParsedURLString, kResourceURL), "text/html", 0, nullAtom, String()));
        memoryCache()->add(resource.get());

        return resource;
    }
