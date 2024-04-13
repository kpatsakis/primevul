InputEventAckState InputMsgWatcher::WaitForAck() {
  DCHECK_CURRENTLY_ON(BrowserThread::UI);
  base::RunLoop run_loop;
  base::AutoReset<base::Closure> reset_quit(&quit_, run_loop.QuitClosure());
  run_loop.Run();
  return ack_result_;
}
