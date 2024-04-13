int DecrementEventListenerCount(ScriptContext* script_context,
                                const std::string& event_name) {
  return --g_listener_counts
               .Get()[GetKeyForScriptContext(script_context)][event_name];
}
