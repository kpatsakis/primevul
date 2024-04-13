void IDNSpoofChecker::RestoreTopDomainGraphToDefault() {
  g_graph = kDafsa;
  g_graph_length = sizeof(kDafsa);
}
