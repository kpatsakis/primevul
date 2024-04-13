void AbortRequestBeforeItStarts(ResourceMessageFilter* filter,
                                IPC::Message* sync_result,
                                int route_id,
                                int request_id) {
  if (sync_result) {
    SyncLoadResult result;
    result.error_code = net::ERR_ABORTED;
    ResourceHostMsg_SyncLoad::WriteReplyParams(sync_result, result);
    filter->Send(sync_result);
  } else {
    filter->Send(new ResourceMsg_RequestComplete(
        route_id,
        request_id,
        net::ERR_ABORTED,
        false,
        std::string(),   // No security info needed, connection not established.
        base::TimeTicks()));
  }
}
