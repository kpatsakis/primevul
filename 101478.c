bool HTMLFormControlElement::IsAutofocusable() const {
  return FastHasAttribute(kAutofocusAttr) && SupportsAutofocus();
}
