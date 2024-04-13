static bool FindAttributeWithName(const HTMLToken& token,
                                  const QualifiedName& name,
                                  size_t& index_of_matching_attribute) {
  const String& attr_name = name.NamespaceURI() == XLinkNames::xlinkNamespaceURI
                                ? "xlink:" + name.LocalName().GetString()
                                : name.LocalName().GetString();

  for (size_t i = 0; i < token.Attributes().size(); ++i) {
    if (EqualIgnoringNullity(token.Attributes().at(i).NameAsVector(),
                             attr_name)) {
      index_of_matching_attribute = i;
      return true;
    }
  }
  return false;
}
