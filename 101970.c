bool SVGElement::IsPresentationAttributeWithSVGDOM(
    const QualifiedName& name) const {
  const SVGAnimatedPropertyBase* property = PropertyFromAttribute(name);
  return property && property->HasPresentationAttributeMapping();
}
