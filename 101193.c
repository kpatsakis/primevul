void Document::registerNodeList(const LiveNodeListBase* list)
{
#if ENABLE(OILPAN)
    m_nodeLists[list->invalidationType()].add(list);
#else
    m_nodeListCounts[list->invalidationType()]++;
#endif
    if (list->isRootedAtDocument())
        m_listsInvalidatedAtDocument.add(list);
}
