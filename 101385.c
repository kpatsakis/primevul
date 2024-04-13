void PPB_Flash_MessageLoop_Impl::InternalQuit(int32_t result) {
  if (!state_->run_called() || state_->quit_called())
    return;
  state_->set_quit_called();
  state_->set_result(result);

  base::MessageLoop::current()->QuitNow();

  if (!state_->run_callback().is_null())
    state_->run_callback().Run(result);
}
