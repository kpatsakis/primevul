bool ExecuteCodeInTabFunction::Execute(const std::string& code_string) {
  content::WebContents* contents = NULL;
  Browser* browser = NULL;

  bool success = GetTabById(
      execute_tab_id_, profile(), include_incognito(), &browser, NULL,
      &contents, NULL, &error_) && contents && browser;

  if (!success)
    return false;

  const extensions::Extension* extension = GetExtension();
  if (!extension)
    return false;

  ScriptExecutor::ScriptType script_type = ScriptExecutor::JAVASCRIPT;
  std::string function_name = name();
  if (function_name == TabsInsertCSSFunction::function_name()) {
    script_type = ScriptExecutor::CSS;
  } else if (function_name != TabsExecuteScriptFunction::function_name()) {
    NOTREACHED();
  }

  ScriptExecutor::FrameScope frame_scope =
      details_->all_frames.get() && *details_->all_frames ?
          ScriptExecutor::ALL_FRAMES :
          ScriptExecutor::TOP_FRAME;

  UserScript::RunLocation run_at = UserScript::UNDEFINED;
  switch (details_->run_at) {
    case InjectDetails::RUN_AT_NONE:
    case InjectDetails::RUN_AT_DOCUMENT_IDLE:
      run_at = UserScript::DOCUMENT_IDLE;
      break;
    case InjectDetails::RUN_AT_DOCUMENT_START:
      run_at = UserScript::DOCUMENT_START;
      break;
    case InjectDetails::RUN_AT_DOCUMENT_END:
      run_at = UserScript::DOCUMENT_END;
      break;
  }
  CHECK_NE(UserScript::UNDEFINED, run_at);

  extensions::TabHelper::FromWebContents(contents)->
      script_executor()->ExecuteScript(
          extension->id(),
          script_type,
          code_string,
          frame_scope,
          run_at,
          ScriptExecutor::ISOLATED_WORLD,
          false /* is_web_view */,
          base::Bind(&ExecuteCodeInTabFunction::OnExecuteCodeFinished, this));
  return true;
}
