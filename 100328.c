static bool contentHeightIsConstrained(const RenderBlock* container)
{
    for (; container; container = container->containingBlock()) {
        RenderStyle* style = container->style();
        if (style->overflowY() >= OSCROLL)
            return false;
        if (style->height().isSpecified() || style->maxHeight().isSpecified()) {
            return !container->isRoot() && !container->isBody();
        }
        if (container->isFloatingOrOutOfFlowPositioned())
            return false;
    }
    return false;
}
