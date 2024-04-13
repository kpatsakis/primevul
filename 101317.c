AXTree::AXTree() {
  AXNodeData root;
  root.id = -1;

  AXTreeUpdate initial_state;
  initial_state.root_id = -1;
  initial_state.nodes.push_back(root);
  CHECK(Unserialize(initial_state)) << error();
}
