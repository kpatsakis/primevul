void HTMLFormElement::removeFromPastNamesMap(HTMLElement& element)
{
    if (!m_pastNamesMap)
        return;
    PastNamesMap::iterator end = m_pastNamesMap->end();
    for (PastNamesMap::iterator it = m_pastNamesMap->begin(); it != end; ++it) {
        if (it->value.get() == &element) {
            it->value = 0;
        }
    }
}
