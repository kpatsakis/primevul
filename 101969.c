Node::InsertionNotificationRequest SVGElement::InsertedInto(
    ContainerNode& root_parent) {
  Element::InsertedInto(root_parent);
  UpdateRelativeLengthsInformation();

  const AtomicString& nonce_value = FastGetAttribute(kNonceAttr);
  if (!nonce_value.IsEmpty()) {
    setNonce(nonce_value);
    if (InActiveDocument() &&
        GetDocument().GetContentSecurityPolicy()->HasHeaderDeliveredPolicy()) {
      setAttribute(kNonceAttr, g_empty_atom);
    }
  }
  return kInsertionDone;
}
