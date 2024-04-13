void DOMMessageQueue::Observe(int type,
                              const NotificationSource& source,
                              const NotificationDetails& details) {
  Details<std::string> dom_op_result(details);
  message_queue_.push(*dom_op_result.ptr());
  if (message_loop_runner_)
    message_loop_runner_->Quit();
}
