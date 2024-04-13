void Document::EnqueueAnimationFrameEvent(Event* event) {
  EnsureScriptedAnimationController().EnqueueEvent(event);
}
