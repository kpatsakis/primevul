void ContainerNode::parserAddChild(PassRefPtr<Node> newChild)
{
    ASSERT(newChild);
    ASSERT(!newChild->parentNode()); // Use appendChild if you need to handle reparenting (and want DOM mutation events).

    forbidEventDispatch();
    Node* last = m_lastChild;
    appendChildToContainer(newChild.get(), this);
    treeScope()->adoptIfNeeded(newChild.get());
    
    allowEventDispatch();

    childrenChanged(true, last, 0, 1);
    ChildNodeInsertionNotifier(this).notify(newChild.get());
}
