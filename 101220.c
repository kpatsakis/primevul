inline void ImageLoader::dispatchErrorEvent() {
  m_hasPendingErrorEvent = true;
  errorEventSender().dispatchEventSoon(this);
}
