AtomicString Document::contentType() const
{
    if (!m_mimeType.isEmpty())
        return m_mimeType;

    if (DocumentLoader* documentLoader = loader())
        return documentLoader->mimeType();

    String mimeType = suggestedMIMEType();
    if (!mimeType.isEmpty())
        return AtomicString(mimeType);

    return AtomicString("application/xml");
}
