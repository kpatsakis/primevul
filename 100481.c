void ChromotingInstance::ProcessLogToUI(const std::string& message) {
  DCHECK(plugin_task_runner_->BelongsToCurrentThread());

  g_logging_to_plugin = true;
  scoped_ptr<base::DictionaryValue> data(new base::DictionaryValue());
  data->SetString("message", message);
  PostChromotingMessage("logDebugMessage", data.Pass());
   g_logging_to_plugin = false;
 }
