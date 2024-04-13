void SVGElement::ParseAttribute(const AttributeModificationParams& params) {
  if (SVGAnimatedPropertyBase* property = PropertyFromAttribute(params.name)) {
    SVGParsingError parse_error = property->AttributeChanged(params.new_value);
    ReportAttributeParsingError(parse_error, params.name, params.new_value);
    return;
  }

  const AtomicString& event_name =
      HTMLElement::EventNameForAttributeName(params.name);
  if (!event_name.IsNull()) {
    SetAttributeEventListener(
        event_name,
        CreateAttributeEventListener(this, params.name, params.new_value));
    return;
  }

  Element::ParseAttribute(params);
}
