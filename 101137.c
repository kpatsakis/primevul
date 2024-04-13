bool SVGAnimateElement::calculateFromAndToValues(const String& fromString, const String& toString)
{
    SVGElement* targetElement = this->targetElement();
    if (!targetElement)
        return false;

    determinePropertyValueTypes(fromString, toString);
    ensureAnimator()->calculateFromAndToValues(m_fromType, m_toType, fromString, toString);
    ASSERT(m_animatedPropertyType == m_animator->type());
    return true;
}
