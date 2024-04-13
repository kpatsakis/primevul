void Element::detachAllAttrNodesFromElement()
{
    AttrNodeList* attrNodeList = attrNodeListForElement(this);
    ASSERT(attrNodeList);

    for (unsigned i = 0; i < attributeCount(); ++i) {
        const Attribute* attribute = attributeItem(i);
        if (RefPtr<Attr> attrNode = findAttrNodeInList(attrNodeList, attribute->name()))
            attrNode->detachFromElementWithValue(attribute->value());
    }

    removeAttrNodeListForElement(this);
}
