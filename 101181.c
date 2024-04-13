void Document::updateHoverActiveState(const HitTestRequest& request, Element* innerElement)
{
    ASSERT(!request.readOnly());

    if (request.active() && m_frame)
        m_frame->eventHandler().notifyElementActivated();

    Element* innerElementInDocument = innerElement;
    while (innerElementInDocument && innerElementInDocument->document() != this) {
        innerElementInDocument->document().updateHoverActiveState(request, innerElementInDocument);
        innerElementInDocument = innerElementInDocument->document().ownerElement();
    }

    updateDistribution();
    Element* oldActiveElement = activeHoverElement();
    if (oldActiveElement && !request.active()) {
        for (RefPtrWillBeRawPtr<Node> node = oldActiveElement; node; node = ComposedTreeTraversal::parent(*node)) {
            ASSERT(!node->isTextNode());
            node->setActive(false);
            m_userActionElements.setInActiveChain(node.get(), false);
        }
        setActiveHoverElement(nullptr);
    } else {
        Element* newActiveElement = innerElementInDocument;
        if (!oldActiveElement && newActiveElement && !newActiveElement->isDisabledFormControl() && request.active() && !request.touchMove()) {
            for (Node* node = newActiveElement; node; node = ComposedTreeTraversal::parent(*node)) {
                ASSERT(!node->isTextNode());
                m_userActionElements.setInActiveChain(node, true);
            }
            setActiveHoverElement(newActiveElement);
        }
    }
    bool allowActiveChanges = !oldActiveElement && activeHoverElement();

    bool mustBeInActiveChain = request.active() && request.move();

    RefPtrWillBeRawPtr<Node> oldHoverNode = hoverNode();

    Node* newHoverNode = innerElementInDocument;
    while (newHoverNode && !newHoverNode->layoutObject())
        newHoverNode = newHoverNode->parentOrShadowHostNode();

    setHoverNode(newHoverNode);

    LayoutObject* oldHoverObj = oldHoverNode ? oldHoverNode->layoutObject() : 0;
    LayoutObject* newHoverObj = newHoverNode ? newHoverNode->layoutObject() : 0;

    LayoutObject* ancestor = nearestCommonHoverAncestor(oldHoverObj, newHoverObj);
    RefPtrWillBeRawPtr<Node> ancestorNode(ancestor ? ancestor->node() : 0);

    WillBeHeapVector<RefPtrWillBeMember<Node>, 32> nodesToRemoveFromChain;
    WillBeHeapVector<RefPtrWillBeMember<Node>, 32> nodesToAddToChain;

    if (oldHoverObj != newHoverObj) {
        if (oldHoverNode && !oldHoverObj) {
            for (Node* node = oldHoverNode.get(); node; node = node->parentNode()) {
                if (!mustBeInActiveChain || (node->isElementNode() && toElement(node)->inActiveChain()))
                    nodesToRemoveFromChain.append(node);
            }

        }

        for (LayoutObject* curr = oldHoverObj; curr && curr != ancestor; curr = curr->hoverAncestor()) {
            if (curr->node() && !curr->isText() && (!mustBeInActiveChain || curr->node()->inActiveChain()))
                nodesToRemoveFromChain.append(curr->node());
        }

    }

    for (LayoutObject* curr = newHoverObj; curr; curr = curr->hoverAncestor()) {
        if (curr->node() && !curr->isText() && (!mustBeInActiveChain || curr->node()->inActiveChain()))
            nodesToAddToChain.append(curr->node());
    }

    size_t removeCount = nodesToRemoveFromChain.size();
    for (size_t i = 0; i < removeCount; ++i) {
        nodesToRemoveFromChain[i]->setHovered(false);
    }

    bool sawCommonAncestor = false;
    size_t addCount = nodesToAddToChain.size();
    for (size_t i = 0; i < addCount; ++i) {
        if (ancestorNode && nodesToAddToChain[i] == ancestorNode)
            sawCommonAncestor = true;
        if (allowActiveChanges)
            nodesToAddToChain[i]->setActive(true);
        if (!sawCommonAncestor || nodesToAddToChain[i] == m_hoverNode) {
            nodesToAddToChain[i]->setHovered(true);
        }
    }
}
