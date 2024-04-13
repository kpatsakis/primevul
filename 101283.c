const AtomicString& Document::dir() {
  Element* root_element = documentElement();
  if (auto* html = ToHTMLHtmlElementOrNull(root_element))
    return html->dir();
  return g_null_atom;
}
