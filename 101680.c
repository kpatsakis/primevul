  MockWidgetInputHandler::MessageVector GetAndResetDispatchedMessages() {
    return widget_host_->input_handler()->GetAndResetDispatchedMessages();
  }
