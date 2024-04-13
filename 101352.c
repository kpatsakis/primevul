void BaseAudioContext::HandleDecodeAudioData(
    AudioBuffer* audio_buffer,
    ScriptPromiseResolver* resolver,
    V8PersistentCallbackFunction<V8DecodeSuccessCallback>* success_callback,
    V8PersistentCallbackFunction<V8DecodeErrorCallback>* error_callback) {
  DCHECK(IsMainThread());

  if (audio_buffer) {
    resolver->Resolve(audio_buffer);
    if (success_callback)
      success_callback->InvokeAndReportException(this, audio_buffer);
  } else {
    DOMException* error = DOMException::Create(DOMExceptionCode::kEncodingError,
                                               "Unable to decode audio data");
    resolver->Reject(error);
    if (error_callback)
      error_callback->InvokeAndReportException(this, error);
  }

  DCHECK(decode_audio_resolvers_.Contains(resolver));
  decode_audio_resolvers_.erase(resolver);
}
