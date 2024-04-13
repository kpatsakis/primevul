ElementData::ElementData(const ElementData& other, bool isUnique)
    : m_isUnique(isUnique)
    , m_arraySize(isUnique ? 0 : other.length())
    , m_presentationAttributeStyleIsDirty(other.m_presentationAttributeStyleIsDirty)
    , m_styleAttributeIsDirty(other.m_styleAttributeIsDirty)
    , m_animatedSVGAttributesAreDirty(other.m_animatedSVGAttributesAreDirty)
    , m_classNames(other.m_classNames)
    , m_idForStyleResolution(other.m_idForStyleResolution)
{
}
