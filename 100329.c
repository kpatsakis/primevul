void TextAutosizer::measureDescendantTextWidth(const RenderBlock* container, float minTextWidth, float& textWidth)
{
    bool skipLocalText = contentHeightIsConstrained(container);

    RenderObject* descendant = nextInPreOrderSkippingDescendantsOfContainers(container, container);
    while (descendant) {
        if (!skipLocalText && descendant->isText()) {
            textWidth += toRenderText(descendant)->renderedTextLength() * descendant->style()->specifiedFontSize();
        } else if (isAutosizingContainer(descendant)) {
            RenderBlock* descendantBlock = toRenderBlock(descendant);
            if (!isAutosizingCluster(descendantBlock))
                measureDescendantTextWidth(descendantBlock, minTextWidth, textWidth);
        }
        if (textWidth >= minTextWidth)
            return;
        descendant = nextInPreOrderSkippingDescendantsOfContainers(descendant, container);
    }
}
