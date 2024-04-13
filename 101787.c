void MimeHandlerViewContainer::CreateMimeHandlerViewGuestIfNecessary() {
  if (guest_created_ || !element_size_.has_value() || view_id_.empty())
    return;

  if (loader_) {
    DCHECK(is_embedded_);
    loader_.reset();
  }

  DCHECK_NE(element_instance_id(), guest_view::kInstanceIDNone);

  if (!render_frame())
    return;

  render_frame()->Send(
      new ExtensionsGuestViewHostMsg_CreateMimeHandlerViewGuest(
          render_frame()->GetRoutingID(), view_id_, element_instance_id(),
          *element_size_));

  guest_created_ = true;
}
