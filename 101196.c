String Document::userAgent(const KURL& url) const
{
    return frame() ? frame()->loader().userAgent(url) : String();
}
