PassRefPtr<HTMLFormElement> HTMLFormElement::create(const QualifiedName& tagName, Document& document)
{
    UseCounter::count(&document, UseCounter::FormElement);
    return adoptRef(new HTMLFormElement(tagName, document));
}
