void HTMLMediaElement::RejectScheduledPlayPromises() {
  DCHECK(play_promise_error_code_ == DOMExceptionCode::kAbortError ||
         play_promise_error_code_ == DOMExceptionCode::kNotSupportedError);
  if (play_promise_error_code_ == DOMExceptionCode::kAbortError) {
    RecordPlayPromiseRejected(PlayPromiseRejectReason::kInterruptedByPause);
    RejectPlayPromisesInternal(DOMExceptionCode::kAbortError,
                               "The play() request was interrupted by a call "
                               "to pause(). https://goo.gl/LdLk22");
  } else {
    RecordPlayPromiseRejected(PlayPromiseRejectReason::kNoSupportedSources);
    RejectPlayPromisesInternal(
        DOMExceptionCode::kNotSupportedError,
        "Failed to load because no supported source was found.");
  }
}
