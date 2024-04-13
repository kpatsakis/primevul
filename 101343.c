bool HTMLMediaElement::SupportsSave() const {
  if (GetDocument().GetSettings() &&
      GetDocument().GetSettings()->GetHideDownloadUI()) {
    return false;
  }

  if (current_src_.IsNull() || current_src_.IsEmpty())
    return false;

  if (network_state_ == kNetworkEmpty || network_state_ == kNetworkNoSource)
    return false;

  if (current_src_.IsLocalFile())
    return false;

  if (GetLoadType() == WebMediaPlayer::kLoadTypeMediaStream)
    return false;

  if (HasMediaSource())
    return false;

  if (IsHLSURL(current_src_))
    return false;

  if (duration() == std::numeric_limits<double>::infinity())
    return false;

  return true;
}
