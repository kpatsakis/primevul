void RenderFrameImpl::showContextMenu(const blink::WebContextMenuData& data) {
  ContextMenuParams params = ContextMenuParamsBuilder::Build(data);
  params.source_type = GetRenderWidget()->context_menu_source_type();
  if (params.source_type == ui::MENU_SOURCE_TOUCH_EDIT_MENU) {
    params.x = GetRenderWidget()->touch_editing_context_menu_location().x();
    params.y = GetRenderWidget()->touch_editing_context_menu_location().y();
  }
  GetRenderWidget()->OnShowHostContextMenu(&params);

  if (ShouldUpdateSelectionTextFromContextMenuParams(
          selection_text_, selection_text_offset_, selection_range_, params)) {
    selection_text_ = params.selection_text;
    selection_text_offset_ = 0;
    selection_range_ = gfx::Range(0, selection_text_.length());
    Send(new ViewHostMsg_SelectionChanged(
        GetRenderWidget()->routing_id(), selection_text_,
        selection_text_offset_, selection_range_));
  }

  if (params.src_url.spec().size() > GetMaxURLChars())
    params.src_url = GURL();
  context_menu_node_ = data.node;

#if defined(OS_ANDROID)
  gfx::Rect start_rect;
  gfx::Rect end_rect;
  GetRenderWidget()->GetSelectionBounds(&start_rect, &end_rect);
  params.selection_start = gfx::Point(start_rect.x(), start_rect.bottom());
  params.selection_end = gfx::Point(end_rect.right(), end_rect.bottom());
#endif

  Send(new FrameHostMsg_ContextMenu(routing_id_, params));
}
