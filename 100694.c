PassRefPtr<HTMLCollection> HTMLFormElement::elements()
{
    return ensureCachedHTMLCollection(FormControls);
}
