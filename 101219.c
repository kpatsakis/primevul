static void configureRequest(
    FetchRequest& request,
    ImageLoader::BypassMainWorldBehavior bypassBehavior,
    Element& element,
    const ClientHintsPreferences& clientHintsPreferences) {
  if (bypassBehavior == ImageLoader::BypassMainWorldCSP)
    request.setContentSecurityCheck(DoNotCheckContentSecurityPolicy);

  CrossOriginAttributeValue crossOrigin = crossOriginAttributeValue(
      element.fastGetAttribute(HTMLNames::crossoriginAttr));
  if (crossOrigin != CrossOriginAttributeNotSet) {
    request.setCrossOriginAccessControl(element.document().getSecurityOrigin(),
                                        crossOrigin);
  }

  if (clientHintsPreferences.shouldSendResourceWidth() &&
      isHTMLImageElement(element))
    request.setResourceWidth(toHTMLImageElement(element).getResourceWidth());
}
