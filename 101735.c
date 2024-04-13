void Document::MaybeQueueSendDidEditFieldInInsecureContext() {
  if (logged_field_edit_ || sensitive_input_edited_task_.IsActive() ||
      IsSecureContext()) {
    return;
  }
  logged_field_edit_ = true;
  sensitive_input_edited_task_ =
      GetTaskRunner(TaskType::kUserInteraction)
          ->PostCancellableTask(
              BLINK_FROM_HERE,
              WTF::Bind(&Document::SendDidEditFieldInInsecureContext,
                        WrapWeakPersistent(this)));
}
