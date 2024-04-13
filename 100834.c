void TextIterator::exitNode()
{
    if (!m_hasEmitted)
        return;
        
    Node* baseNode = m_node->lastChild() ? m_node->lastChild() : m_node;
    if (m_lastTextNode && shouldEmitNewlineAfterNode(m_node)) {
        bool addNewline = shouldEmitExtraNewlineForNode(m_node);
        
        if (m_lastCharacter != '\n') {
            emitCharacter('\n', baseNode->parentNode(), baseNode, 1, 1);
            ASSERT(!m_needsAnotherNewline);
            m_needsAnotherNewline = addNewline;
        } else if (addNewline)
            emitCharacter('\n', baseNode->parentNode(), baseNode, 1, 1);
    }
    
    if (!m_positionNode && shouldEmitSpaceBeforeAndAfterNode(m_node))
        emitCharacter(' ', baseNode->parentNode(), baseNode, 1, 1);
}
