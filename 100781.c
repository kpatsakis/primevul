PassRefPtr<ShareableElementData> UniqueElementData::makeShareableCopy() const
{
    void* slot = WTF::fastMalloc(sizeForShareableElementDataWithAttributeCount(m_attributeVector.size()));
    return adoptRef(new (slot) ShareableElementData(*this));
}
