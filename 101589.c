void HTMLLinkElement::StartLoadingDynamicSheet() {
  DCHECK(GetLinkStyle());
  GetLinkStyle()->StartLoadingDynamicSheet();
}
