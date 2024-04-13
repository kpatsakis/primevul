bool EditorClientBlackBerry::shouldSpellCheckFocusedField()
{
    const Frame* frame = m_webPagePrivate->focusedOrMainFrame();
    if (!frame || !frame->document() || !frame->editor())
        return false;

    const Node* node = frame->document()->focusedNode();
    if (!node)
        return true;

    if (node->isElementNode()) {
        const Element* element = static_cast<const Element*>(node);
        if (element->hasTagName(HTMLNames::inputTag) && !DOMSupport::elementSupportsAutocomplete(element))
            return false;
    }

    return frame->editor()->isSpellCheckingEnabledInFocusedNode();
}
