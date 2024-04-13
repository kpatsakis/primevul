int ResourceDispatcherHostImpl::BuildLoadFlagsForRequest(
    const ResourceHostMsg_Request& request_data,
    int child_id,
    bool is_sync_load) {
  int load_flags = request_data.load_flags;

  load_flags |= net::LOAD_VERIFY_EV_CERT;
  if (request_data.resource_type == RESOURCE_TYPE_MAIN_FRAME) {
    load_flags |= net::LOAD_MAIN_FRAME;
  } else if (request_data.resource_type == RESOURCE_TYPE_PREFETCH) {
    load_flags |= net::LOAD_PREFETCH;
  }

  if (is_sync_load)
    load_flags |= net::LOAD_IGNORE_LIMITS;

  return load_flags;
}
