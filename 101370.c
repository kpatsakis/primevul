void RenderWidgetHostImpl::LostMouseLock() {
  Send(new ViewMsg_MouseLockLost(routing_id_));

  if (delegate_)
    delegate_->LostMouseLock(this);
}
