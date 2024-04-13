void Element::detachAttrNodeAtIndex(Attr* attr, size_t index)
{
    ASSERT(attr);
    ASSERT(elementData());

    const Attribute* attribute = elementData()->attributeItem(index);
    ASSERT(attribute);
    ASSERT(attribute->name() == attr->qualifiedName());
    detachAttrNodeFromElementWithValue(attr, attribute->value());
    removeAttributeInternal(index, NotInSynchronizationOfLazyAttribute);
}
