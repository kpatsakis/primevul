void PasswordAutofillAgent::FrameDetached() {
  if (render_frame()->GetWebFrame()->parent() &&
      ProvisionallySavedPasswordIsValid()) {
    Send(new AutofillHostMsg_InPageNavigation(routing_id(),
                                              *provisionally_saved_form_));
  }
  FrameClosing();
}
