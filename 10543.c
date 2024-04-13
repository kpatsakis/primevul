void LinkResolver::resolve_invokespecial(CallInfo& result, Handle recv,
                                         const constantPoolHandle& pool, int index, TRAPS) {
  LinkInfo link_info(pool, index, CHECK);
  resolve_special_call(result, recv, link_info, CHECK);
}