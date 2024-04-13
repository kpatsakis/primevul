const AtomicString& Document::dir()
{
    Element* rootElement = documentElement();
    if (isHTMLHtmlElement(rootElement))
        return toHTMLHtmlElement(rootElement)->dir();
    return nullAtom;
}
