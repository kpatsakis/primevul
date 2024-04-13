bool HTMLTextAreaElement::matchesReadWritePseudoClass() const
{
    return !isReadOnly();
}
