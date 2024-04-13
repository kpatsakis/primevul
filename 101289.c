void Document::setTitle(const String& title) {
  Element* element = documentElement();
  if (IsSVGSVGElement(element)) {
    if (!title_element_) {
      title_element_ = SVGTitleElement::Create(*this);
      element->InsertBefore(title_element_.Get(), element->firstChild());
    }
    if (auto* svg_title = ToSVGTitleElementOrNull(title_element_))
      svg_title->SetText(title);
  } else if (element && element->IsHTMLElement()) {
    if (!title_element_) {
      HTMLElement* head_element = head();
      if (!head_element)
        return;
      title_element_ = HTMLTitleElement::Create(*this);
      head_element->AppendChild(title_element_.Get());
    }
    if (auto* html_title = ToHTMLTitleElementOrNull(title_element_))
      html_title->setText(title);
  }
}
