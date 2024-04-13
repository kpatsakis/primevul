scoped_refptr<AudioOutputController> AudioOutputController::Create(
    EventHandler* event_handler,
    AudioParameters params,
    uint32 buffer_capacity) {

  if (!CheckParameters(params))
    return NULL;

  if (!AudioManager::GetAudioManager())
    return NULL;

  scoped_refptr<AudioOutputController> controller(new AudioOutputController(
      event_handler, buffer_capacity, NULL));

  controller->message_loop_ =
      AudioManager::GetAudioManager()->GetMessageLoop();
  controller->message_loop_->PostTask(
      FROM_HERE,
      NewRunnableMethod(controller.get(), &AudioOutputController::DoCreate,
                        params));
  return controller;
}
