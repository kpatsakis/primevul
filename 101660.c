void RenderWidgetHostImpl::DragTargetDrop(const DropData& drop_data,
                                          const gfx::PointF& client_pt,
                                          const gfx::PointF& screen_pt,
                                          int key_modifiers) {
  DropData drop_data_with_permissions(drop_data);
  GrantFileAccessFromDropData(&drop_data_with_permissions);
  Send(new DragMsg_TargetDrop(GetRoutingID(), drop_data_with_permissions,
                              client_pt, screen_pt, key_modifiers));
}
