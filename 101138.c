PassRefPtr<SVGAnimateElement> SVGAnimateElement::create(Document& document)
{
    return adoptRef(new SVGAnimateElement(SVGNames::animateTag, document));
}
