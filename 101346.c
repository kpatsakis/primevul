void HTMLMediaElement::AudioSourceProviderImpl::Wrap(
    WebAudioSourceProvider* provider) {
  MutexLocker locker(provide_input_lock);

  if (web_audio_source_provider_ && provider != web_audio_source_provider_)
    web_audio_source_provider_->SetClient(nullptr);

  web_audio_source_provider_ = provider;
  if (web_audio_source_provider_)
    web_audio_source_provider_->SetClient(client_.Get());
}
