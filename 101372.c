void RenderWidgetHostImpl::OnHittestData(
    const FrameHostMsg_HittestData_Params& params) {
  if (delegate_)
    delegate_->GetInputEventRouter()->OnHittestData(params);
}
