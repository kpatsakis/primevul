void Element::setChildrenAffectedByActive(bool value)
{
    if (value || hasRareData())
        ensureElementRareData()->setChildrenAffectedByActive(value);
}
