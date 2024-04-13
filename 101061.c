void ResourceFetcher::subresourceLoaderFinishedLoadingOnePart(ResourceLoader* loader)
{
    if (!m_multipartLoaders)
        m_multipartLoaders = adoptPtr(new ResourceLoaderSet());
    m_multipartLoaders->add(loader);
    m_loaders->remove(loader);
    if (LocalFrame* frame = this->frame())
        return frame->loader().checkLoadComplete();
}
