bool AXTree::CreateNewChildVector(AXNode* node,
                                  const std::vector<int32_t>& new_child_ids,
                                  std::vector<AXNode*>* new_children,
                                  AXTreeUpdateState* update_state) {
  bool success = true;
  for (size_t i = 0; i < new_child_ids.size(); ++i) {
    int32_t child_id = new_child_ids[i];
    int32_t index_in_parent = static_cast<int32_t>(i);
    AXNode* child = GetFromId(child_id);
    if (child) {
      if (child->parent() != node) {
        error_ = base::StringPrintf(
            "Node %d reparented from %d to %d",
            child->id(),
            child->parent() ? child->parent()->id() : 0,
            node->id());
        success = false;
        continue;
      }
      child->SetIndexInParent(index_in_parent);
    } else {
      child = CreateNode(node, child_id, index_in_parent, update_state);
      update_state->pending_nodes.insert(child);
      update_state->new_nodes.insert(child);
    }
    new_children->push_back(child);
  }

  return success;
}
