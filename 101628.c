std::string SessionStore::GetTabClientTagForTest(const std::string& session_tag,
                                                 int tab_node_id) {
  return TabNodeIdToClientTag(session_tag, tab_node_id);
}
