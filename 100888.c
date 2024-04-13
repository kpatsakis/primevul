String HTMLTextAreaElement::defaultValue() const
{
    StringBuilder value;

    for (Node* n = firstChild(); n; n = n->nextSibling()) {
        if (n->isTextNode())
            value.append(toText(n)->data());
    }

    return value.toString();
}
