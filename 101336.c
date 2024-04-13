void HTMLMediaElement::ExecuteDeferredLoad() {
  DCHECK_GE(deferred_load_state_, kWaitingForTrigger);


  CancelDeferredLoad();
  SetShouldDelayLoadEvent(true);
  SetNetworkState(kNetworkLoading);

  StartProgressEventTimer();

  StartPlayerLoad();
}
