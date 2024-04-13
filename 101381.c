void RenderWidgetHostImpl::SetView(RenderWidgetHostViewBase* view) {
  if (view)
    view_ = view->GetWeakPtr();
  else
    view_.reset();

  if (view_ && renderer_initialized_) {
    Send(new ViewMsg_SetSurfaceIdNamespace(routing_id_,
                                           view_->GetSurfaceIdNamespace()));
  }

  synthetic_gesture_controller_.reset();
}
