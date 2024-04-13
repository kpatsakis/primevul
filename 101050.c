bool ResourceFetcher::clientDefersImage(const KURL& url) const
{
    return frame() && !frame()->loader().client()->allowImage(m_imagesEnabled, url);
}
