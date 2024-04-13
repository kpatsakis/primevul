  void Wait() {
    if (!AreAllFramesInTab())
      message_loop_runner_->Run();
  }
