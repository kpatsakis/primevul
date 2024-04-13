Element* Document::CreateElement(const QualifiedName& q_name,
                                 const CreateElementFlags flags,
                                 const AtomicString& is) {
  CustomElementDefinition* definition = nullptr;
  if (flags.IsCustomElementsV1() &&
      q_name.NamespaceURI() == html_names::xhtmlNamespaceURI) {
    const CustomElementDescriptor desc(is.IsNull() ? q_name.LocalName() : is,
                                       q_name.LocalName());
    if (CustomElementRegistry* registry = CustomElement::Registry(*this))
      definition = registry->DefinitionFor(desc);
  }

  if (definition)
    return definition->CreateElement(*this, q_name, flags);

  return CustomElement::CreateUncustomizedOrUndefinedElement(*this, q_name,
                                                             flags, is);
}
