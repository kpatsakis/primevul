void HTMLMediaElement::MediaControlsDidBecomeVisible() {
  BLINK_MEDIA_LOG << "mediaControlsDidBecomeVisible(" << (void*)this << ")";

  if (IsHTMLVideoElement() && TextTracksVisible()) {
    EnsureTextTrackContainer().UpdateDisplay(
        *this, TextTrackContainer::kDidStartExposingControls);
  }
}
