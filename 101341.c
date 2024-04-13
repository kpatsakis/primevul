void HTMLMediaElement::RemotePlaybackStarted() {
  if (RemotePlaybackClient())
    RemotePlaybackClient()->StateChanged(WebRemotePlaybackState::kConnected);
}
