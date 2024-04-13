bool ExecuteScriptWithoutUserGestureAndExtractInt(
    const ToRenderFrameHost& adapter,
    const std::string& script,
    int* result) {
  DCHECK(result);
  std::unique_ptr<base::Value> value;
  return ExecuteScriptHelper(adapter.render_frame_host(), script, false,
                             &value) &&
         value && value->GetAsInteger(result);
}
