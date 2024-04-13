    CompositingRecursionData(RenderLayer* compAncestor, RenderLayer* mostRecentCompositedLayer, bool testOverlap)
        : m_compositingAncestor(compAncestor)
        , m_mostRecentCompositedLayer(mostRecentCompositedLayer)
        , m_subtreeIsCompositing(false)
        , m_hasUnisolatedCompositedBlendingDescendant(false)
        , m_testingOverlap(testOverlap)
#ifndef NDEBUG
        , m_depth(0)
#endif
    {
    }
