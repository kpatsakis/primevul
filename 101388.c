 void TestFlashMessageLoop::QuitMessageLoopTask(int32_t unused) {
   if (message_loop_)
     message_loop_->Quit();
  else
    PP_NOTREACHED();
}
