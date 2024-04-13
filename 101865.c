MojoResult Core::CreateWatcher(MojoWatcherCallback callback,
                               MojoHandle* watcher_handle) {
  RequestContext request_context;
  if (!watcher_handle)
    return MOJO_RESULT_INVALID_ARGUMENT;
  *watcher_handle = AddDispatcher(new WatcherDispatcher(callback));
  if (*watcher_handle == MOJO_HANDLE_INVALID)
    return MOJO_RESULT_RESOURCE_EXHAUSTED;
  return MOJO_RESULT_OK;
}
