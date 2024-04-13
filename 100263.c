void RenderView::OnScriptEvalRequest(const std::wstring& frame_xpath,
                                     const std::wstring& jscript) {
  EvaluateScript(frame_xpath, jscript);
}
