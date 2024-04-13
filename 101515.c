bool HTMLFormControlElement::isDisabledOrReadOnly() const {
  return isDisabledFormControl() || isReadOnly();
}
