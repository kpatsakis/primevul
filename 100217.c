static void* openFunc(const char* uri)
{
    ASSERT(XMLTokenizerScope::currentDocLoader);
    ASSERT(currentThread() == libxmlLoaderThread);

    KURL url(KURL(), uri);

    if (!shouldAllowExternalLoad(url))
        return &globalDescriptor;

    ResourceError error;
    ResourceResponse response;
    Vector<char> data;


    {
        DocLoader* docLoader = XMLTokenizerScope::currentDocLoader;
        XMLTokenizerScope scope(0);

        if (docLoader->frame()) 
            docLoader->frame()->loader()->loadResourceSynchronously(url, AllowStoredCredentials, error, response, data);
    }

    if (!shouldAllowExternalLoad(response.url()))
        return &globalDescriptor;

    return new OffsetBuffer(data);
}
