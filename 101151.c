void Document::cloneDataFromDocument(const Document& other)
{
    setCompatibilityMode(other.compatibilityMode());
    setEncodingData(other.m_encodingData);
    setContextFeatures(other.contextFeatures());
    setSecurityOrigin(other.securityOrigin()->isolatedCopy());
    setMimeType(other.contentType());
}
