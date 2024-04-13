bool ContainerNode::checkParserAcceptChild(const Node& newChild) const
{
    if (!isDocumentNode())
        return true;
    return toDocument(*this).canAcceptChild(newChild, nullptr, IGNORE_EXCEPTION);
}
