static inline void applyCSSPropertyToTarget(SVGElement* targetElement, CSSPropertyID id, const String& value)
{
    ASSERT_WITH_SECURITY_IMPLICATION(!targetElement->m_deletionHasBegun);

    MutableStylePropertySet* propertySet = targetElement->ensureAnimatedSMILStyleProperties();
    if (!propertySet->setProperty(id, value, false, 0))
        return;

    targetElement->setNeedsStyleRecalc(LocalStyleChange);
}
