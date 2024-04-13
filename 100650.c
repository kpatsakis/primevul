void RenderFrameImpl::OnSelectRange(const gfx::Point& start,
                                    const gfx::Point& end) {
  Send(new ViewHostMsg_SelectRange_ACK(GetRenderWidget()->routing_id()));

  base::AutoReset<bool> handling_select_range(&handling_select_range_, true);
  frame_->selectRange(start, end);
}
