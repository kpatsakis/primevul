static inline void applyCSSPropertyToTargetAndInstances(SVGElement* targetElement, const QualifiedName& attributeName, const String& valueAsString)
{
    ASSERT(targetElement);
    if (attributeName == anyQName() || !targetElement->inDocument() || !targetElement->parentNode())
        return;

    CSSPropertyID id = cssPropertyID(attributeName.localName());

    SVGElementInstance::InstanceUpdateBlocker blocker(targetElement);
    applyCSSPropertyToTarget(targetElement, id, valueAsString);

    const HashSet<SVGElementInstance*>& instances = targetElement->instancesForElement();
    const HashSet<SVGElementInstance*>::const_iterator end = instances.end();
    for (HashSet<SVGElementInstance*>::const_iterator it = instances.begin(); it != end; ++it) {
        if (SVGElement* shadowTreeElement = (*it)->shadowTreeElement())
            applyCSSPropertyToTarget(shadowTreeElement, id, valueAsString);
    }
}
