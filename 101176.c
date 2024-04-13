bool shouldInvalidateNodeListCachesForAttr(const HeapHashSet<WeakMember<const LiveNodeListBase>> nodeLists[], const QualifiedName& attrName)
{
    if (!nodeLists[type].isEmpty() && LiveNodeListBase::shouldInvalidateTypeOnAttributeChange(static_cast<NodeListInvalidationType>(type), attrName))
        return true;
    return shouldInvalidateNodeListCachesForAttr<type + 1>(nodeLists, attrName);
}
