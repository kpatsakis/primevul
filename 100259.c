void RenderView::OnExecuteCode(const ViewMsg_ExecuteCode_Params& params) {
  WebFrame* main_frame = webview() ? webview()->mainFrame() : NULL;
  if (!main_frame) {
    Send(new ViewMsg_ExecuteCodeFinished(routing_id_, params.request_id,
                                         false));
    return;
  }

  WebDataSource* ds = main_frame->dataSource();
  NavigationState* navigation_state = NavigationState::FromDataSource(ds);
  if (!navigation_state->user_script_idle_scheduler()->has_run()) {
    pending_code_execution_queue_.push(
        linked_ptr<ViewMsg_ExecuteCode_Params>(
            new ViewMsg_ExecuteCode_Params(params)));
    return;
  }

  ExecuteCodeImpl(main_frame, params);
}
