bool SVGDocumentExtensions::isElementPendingResource(Element* element, const AtomicString& id) const
{
    ASSERT(element);

    if (!hasPendingResource(id))
        return false;

    return m_pendingResources.get(id)->contains(element);
}
