void BaseAudioContext::NotifyWorkletIsReady() {
  DCHECK(IsMainThread());
  DCHECK(audioWorklet()->IsReady());

  {
    GraphAutoLocker locker(this);

    audio_worklet_thread_ =
        audioWorklet()->GetMessagingProxy()->GetBackingWorkerThread();
  }

  if (ContextState() != kClosed) {
    destination()->GetAudioDestinationHandler().RestartRendering();
  }
}
