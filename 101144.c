static bool isSemicolonSeparatedAttribute(const HTMLToken::Attribute& attribute)
{
    return threadSafeMatch(attribute.name, SVGNames::valuesAttr);
}
