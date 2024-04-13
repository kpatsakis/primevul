void Element::createPseudoElementIfNeeded(PseudoId pseudoId)
{
    if (!document()->styleSheetCollection()->usesBeforeAfterRules())
        return;

    if (!renderer() || !pseudoElementRendererIsNeeded(renderer()->getCachedPseudoStyle(pseudoId)))
        return;

    if (!renderer()->canHaveGeneratedChildren())
        return;

    ASSERT(!isPseudoElement());
    RefPtr<PseudoElement> element = PseudoElement::create(this, pseudoId);
    element->attach();
    ensureElementRareData()->setPseudoElement(pseudoId, element.release());
}
