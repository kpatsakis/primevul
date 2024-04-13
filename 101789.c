void MimeHandlerViewContainer::OnCreateMimeHandlerViewGuestACK(
    int element_instance_id) {
  DCHECK_NE(this->element_instance_id(), guest_view::kInstanceIDNone);
  DCHECK_EQ(this->element_instance_id(), element_instance_id);

  if (!render_frame())
    return;

  render_frame()->AttachGuest(element_instance_id);
}
