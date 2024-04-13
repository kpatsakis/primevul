AtomicString Document::ConvertLocalName(const AtomicString& name) {
  return IsHTMLDocument() ? name.LowerASCII() : name;
}
