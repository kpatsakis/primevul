 void SVGDocumentExtensions::pauseAnimations()
{
    WillBeHeapHashSet<RawPtrWillBeMember<SVGSVGElement> >::iterator end = m_timeContainers.end();
    for (WillBeHeapHashSet<RawPtrWillBeMember<SVGSVGElement> >::iterator itr = m_timeContainers.begin(); itr != end; ++itr)
        (*itr)->pauseAnimations();
}
