void DebuggerFunction::FormatErrorMessage(const std::string& format) {
  if (debuggee_.tab_id)
    error_ = ErrorUtils::FormatErrorMessage(
        format, debugger_api_constants::kTabTargetType,
        base::IntToString(*debuggee_.tab_id));
  else if (debuggee_.extension_id)
    error_ = ErrorUtils::FormatErrorMessage(
        format, debugger_api_constants::kBackgroundPageTargetType,
        *debuggee_.extension_id);
  else
    error_ = ErrorUtils::FormatErrorMessage(
        format, debugger_api_constants::kOpaqueTargetType,
        *debuggee_.target_id);
}
