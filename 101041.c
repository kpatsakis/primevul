void SVGDocumentExtensions::rebuildAllElementReferencesForTarget(SVGElement* referencedElement)
{
    ASSERT(referencedElement);
    HashMap<SVGElement*, OwnPtr<HashSet<SVGElement*> > >::iterator it = m_elementDependencies.find(referencedElement);
    if (it == m_elementDependencies.end())
        return;
    ASSERT(it->key == referencedElement);
    Vector<SVGElement*> toBeNotified;

    HashSet<SVGElement*>* referencingElements = it->value.get();
    HashSet<SVGElement*>::iterator setEnd = referencingElements->end();
    for (HashSet<SVGElement*>::iterator setIt = referencingElements->begin(); setIt != setEnd; ++setIt)
        toBeNotified.append(*setIt);

    Vector<SVGElement*>::iterator vectorEnd = toBeNotified.end();
    for (Vector<SVGElement*>::iterator vectorIt = toBeNotified.begin(); vectorIt != vectorEnd; ++vectorIt) {
        if (HashSet<SVGElement*>* referencingElements = setOfElementsReferencingTarget(referencedElement)) {
            if (referencingElements->contains(*vectorIt))
                (*vectorIt)->svgAttributeChanged(XLinkNames::hrefAttr);
        }
    }
}
