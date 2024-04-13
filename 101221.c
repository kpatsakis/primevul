void ImageLoader::setImageWithoutConsideringPendingLoadEvent(
    ImageResourceContent* newImage) {
  DCHECK(m_failedLoadURL.isEmpty());
  ImageResourceContent* oldImage = m_image.get();
  if (newImage != oldImage) {
    m_image = newImage;
    if (m_hasPendingLoadEvent) {
      loadEventSender().cancelEvent(this);
      m_hasPendingLoadEvent = false;
    }
    if (m_hasPendingErrorEvent) {
      errorEventSender().cancelEvent(this);
      m_hasPendingErrorEvent = false;
    }
    m_imageComplete = true;
    if (newImage) {
      newImage->addObserver(this);
    }
    if (oldImage) {
      oldImage->removeObserver(this);
    }
  }

  if (LayoutImageResource* imageResource = layoutImageResource())
    imageResource->resetAnimation();
}
