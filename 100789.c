void Element::updateNamedItemRegistration(const AtomicString& oldName, const AtomicString& newName)
{
    if (!document()->isHTMLDocument())
        return;

    if (!oldName.isEmpty())
        toHTMLDocument(document())->removeNamedItem(oldName);

    if (!newName.isEmpty())
        toHTMLDocument(document())->addNamedItem(newName);
}
