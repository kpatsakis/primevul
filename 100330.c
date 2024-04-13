void TextAutosizer::processCluster(RenderBlock* cluster, RenderBlock* container, RenderObject* subtreeRoot, const TextAutosizingWindowInfo& windowInfo)
{
    ASSERT(isAutosizingCluster(cluster));

    RenderBlock* lowestCommonAncestor = cluster;
    float commonAncestorWidth = lowestCommonAncestor->contentLogicalWidth();

    float multiplier = 1;
    if (clusterShouldBeAutosized(lowestCommonAncestor, commonAncestorWidth)) {
        int logicalWindowWidth = cluster->isHorizontalWritingMode() ? windowInfo.windowSize.width() : windowInfo.windowSize.height();
        int logicalLayoutWidth = cluster->isHorizontalWritingMode() ? windowInfo.minLayoutSize.width() : windowInfo.minLayoutSize.height();
        float logicalClusterWidth = std::min<float>(commonAncestorWidth, logicalLayoutWidth);

        multiplier = logicalClusterWidth / logicalWindowWidth;
        multiplier *= m_document->settings()->textAutosizingFontScaleFactor();
        multiplier = std::max(1.0f, multiplier);
    }

    processContainer(multiplier, container, subtreeRoot, windowInfo);
}
