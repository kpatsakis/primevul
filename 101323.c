AXTree::~AXTree() {
  if (root_)
    DestroyNodeAndSubtree(root_, nullptr);
  for (auto& entry : table_info_map_)
    delete entry.second;
  table_info_map_.clear();
}
