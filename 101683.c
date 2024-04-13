  const IPC::Message* RunAndReturnIPCSent(const base::Closure closure,
                                          MockRenderProcessHost* process,
                                          int32_t message_id) {
    process->sink().ClearMessages();
    closure.Run();
    return process->sink().GetFirstMessageMatching(message_id);
  }
