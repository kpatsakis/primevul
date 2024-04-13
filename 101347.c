void HTMLMediaElement::setPlaybackRate(double rate,
                                       ExceptionState& exception_state) {
  BLINK_MEDIA_LOG << "setPlaybackRate(" << (void*)this << ", " << rate << ")";
  if (GetLoadType() == WebMediaPlayer::kLoadTypeMediaStream)
    return;

  if (rate != 0.0 && (rate < kMinRate || rate > kMaxRate)) {
    UseCounter::Count(GetDocument(),
                      WebFeature::kHTMLMediaElementMediaPlaybackRateOutOfRange);

    exception_state.ThrowDOMException(
        DOMExceptionCode::kNotSupportedError,
        "The provided playback rate (" + String::Number(rate) +
            ") is not in the " + "supported playback range.");

    return;
  }

  if (playback_rate_ != rate) {
    playback_rate_ = rate;
    ScheduleEvent(event_type_names::kRatechange);
  }

  UpdatePlaybackRate();
}
