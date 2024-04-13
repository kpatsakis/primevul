void RenderView::OnUserScriptIdleTriggered(WebFrame* frame) {
  if (RenderThread::current()) {  // Will be NULL during unit tests.
    RenderThread::current()->user_script_slave()->InjectScripts(
        frame, UserScript::DOCUMENT_IDLE);
  }

  WebFrame* main_frame = webview()->mainFrame();
  if (frame == main_frame) {
    while (!pending_code_execution_queue_.empty()) {
      linked_ptr<ViewMsg_ExecuteCode_Params>& params =
          pending_code_execution_queue_.front();
      ExecuteCodeImpl(main_frame, *params);
      pending_code_execution_queue_.pop();
    }
  }
}
