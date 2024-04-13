static inline void removeCSSPropertyFromTargetAndInstances(SVGElement* targetElement, const QualifiedName& attributeName)
{
    ASSERT(targetElement);
    if (attributeName == anyQName() || !targetElement->inDocument() || !targetElement->parentNode())
        return;

    CSSPropertyID id = cssPropertyID(attributeName.localName());

    SVGElementInstance::InstanceUpdateBlocker blocker(targetElement);
    removeCSSPropertyFromTarget(targetElement, id);

    const HashSet<SVGElementInstance*>& instances = targetElement->instancesForElement();
    const HashSet<SVGElementInstance*>::const_iterator end = instances.end();
    for (HashSet<SVGElementInstance*>::const_iterator it = instances.begin(); it != end; ++it) {
        if (SVGElement* shadowTreeElement = (*it)->shadowTreeElement())
            removeCSSPropertyFromTarget(shadowTreeElement, id);
    }
}
