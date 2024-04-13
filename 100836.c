static bool shouldEmitNewlineAfterNode(Node* node)
{
    if (!shouldEmitNewlinesBeforeAndAfterNode(node))
        return false;
    while ((node = NodeTraversal::nextSkippingChildren(node)))
        if (node->renderer())
            return true;
    return false;
}
