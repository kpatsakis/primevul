void HTMLLinkElement::RemovedFrom(ContainerNode& insertion_point) {
  bool was_connected = isConnected();
  HTMLElement::RemovedFrom(insertion_point);
  if (!insertion_point.isConnected())
    return;

  link_loader_->Abort();

  if (!was_connected) {
    DCHECK(!GetLinkStyle() || !GetLinkStyle()->HasSheet());
    return;
  }
  GetDocument().GetStyleEngine().RemoveStyleSheetCandidateNode(*this,
                                                               insertion_point);
  if (link_)
    link_->OwnerRemoved();
}
