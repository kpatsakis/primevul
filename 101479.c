bool HTMLFormControlElement::IsDisabledFormControl() const {
  if (GetDocument().Fetcher()->Archive())
    return true;

  return IsActuallyDisabled();
}
