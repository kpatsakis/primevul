bool Document::dirtyElementsForLayerUpdate()
{
    if (m_layerUpdateSVGFilterElements.isEmpty())
        return false;

    for (Element* element : m_layerUpdateSVGFilterElements)
        element->setNeedsStyleRecalc(LocalStyleChange, StyleChangeReasonForTracing::create(StyleChangeReason::SVGFilterLayerUpdate));
    m_layerUpdateSVGFilterElements.clear();
    return true;
}
