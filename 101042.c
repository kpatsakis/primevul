void SVGDocumentExtensions::removeElementFromPendingResources(Element* element)
{
    ASSERT(element);

    if (!m_pendingResources.isEmpty() && element->hasPendingResources()) {
        Vector<AtomicString> toBeRemoved;
        HashMap<AtomicString, OwnPtr<SVGPendingElements> >::iterator end = m_pendingResources.end();
        for (HashMap<AtomicString, OwnPtr<SVGPendingElements> >::iterator it = m_pendingResources.begin(); it != end; ++it) {
            SVGPendingElements* elements = it->value.get();
            ASSERT(elements);
            ASSERT(!elements->isEmpty());

            elements->remove(element);
            if (elements->isEmpty())
                toBeRemoved.append(it->key);
        }

        clearHasPendingResourcesIfPossible(element);

        Vector<AtomicString>::iterator vectorEnd = toBeRemoved.end();
        for (Vector<AtomicString>::iterator it = toBeRemoved.begin(); it != vectorEnd; ++it)
            removePendingResource(*it);
    }

    if (!m_pendingResourcesForRemoval.isEmpty()) {
        Vector<AtomicString> toBeRemoved;
        HashMap<AtomicString, OwnPtr<SVGPendingElements> >::iterator end = m_pendingResourcesForRemoval.end();
        for (HashMap<AtomicString, OwnPtr<SVGPendingElements> >::iterator it = m_pendingResourcesForRemoval.begin(); it != end; ++it) {
            SVGPendingElements* elements = it->value.get();
            ASSERT(elements);
            ASSERT(!elements->isEmpty());

            elements->remove(element);
            if (elements->isEmpty())
                toBeRemoved.append(it->key);
        }

        Vector<AtomicString>::iterator vectorEnd = toBeRemoved.end();
        for (Vector<AtomicString>::iterator it = toBeRemoved.begin(); it != vectorEnd; ++it)
            removePendingResourceForRemoval(*it);
    }
}
