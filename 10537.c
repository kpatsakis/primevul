void LinkResolver::resolve_invokehandle(CallInfo& result, const constantPoolHandle& pool, int index, TRAPS) {
  // This guy is reached from InterpreterRuntime::resolve_invokehandle.
  LinkInfo link_info(pool, index, CHECK);
  if (TraceMethodHandles) {
    ResourceMark rm(THREAD);
    tty->print_cr("resolve_invokehandle %s %s", link_info.name()->as_C_string(),
                  link_info.signature()->as_C_string());
  }
  resolve_handle_call(result, link_info, CHECK);
}