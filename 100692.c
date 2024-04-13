PassRefPtr<HTMLFormElement> HTMLFormElement::create(Document& document)
{
    UseCounter::count(&document, UseCounter::FormElement);
    return adoptRef(new HTMLFormElement(formTag, document));
}
