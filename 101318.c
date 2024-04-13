AXTree::AXTree(const AXTreeUpdate& initial_state) {
  CHECK(Unserialize(initial_state)) << error();
}
