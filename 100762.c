bool Element::childShouldCreateRenderer(const NodeRenderingContext& childContext) const
{
    if (childContext.node()->isSVGElement())
        return childContext.node()->hasTagName(SVGNames::svgTag) || isSVGElement();

    return ContainerNode::childShouldCreateRenderer(childContext);
}
