void HTMLSelectElement::add(HTMLElement* element, HTMLElement* before, ExceptionState& exceptionState)
{
    RefPtr<HTMLElement> protectNewChild(element);

    if (!element || !(element->hasLocalName(optionTag) || element->hasLocalName(hrTag)))
        return;

    insertBefore(element, before, exceptionState);
    setNeedsValidityCheck();
}
