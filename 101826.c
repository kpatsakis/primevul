void BrowserChildProcessHostImpl::BindInterface(
    const std::string& interface_name,
    mojo::ScopedMessagePipeHandle interface_pipe) {
  DCHECK_CURRENTLY_ON(BrowserThread::IO);
  if (!child_connection_)
    return;

  child_connection_->BindInterface(interface_name, std::move(interface_pipe));
}
