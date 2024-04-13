String HTMLInputElement::altText() const
{
    String alt = fastGetAttribute(altAttr);
    if (alt.isNull())
        alt = getAttribute(titleAttr);
    if (alt.isNull())
        alt = getAttribute(valueAttr);
    if (alt.isEmpty())
        alt = locale().queryString(blink::WebLocalizedString::InputElementAltText);
    return alt;
}
