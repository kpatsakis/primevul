bool ImageLoader::HasPendingEvent() const {
  if (image_content_ && !image_complete_ && !loading_image_document_)
    return true;

  if (pending_load_event_.IsActive() || pending_error_event_.IsActive())
    return true;

  return false;
}
