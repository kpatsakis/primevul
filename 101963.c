void SVGElement::AttachLayoutTree(AttachContext& context) {
  Element::AttachLayoutTree(context);
  if (SVGElement* element = CorrespondingElement())
    element->MapInstanceToElement(this);
}
