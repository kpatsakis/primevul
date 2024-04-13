FrameTreeNode* WebContentsImpl::FindFrameTreeNodeByID(int64 frame_id) {
  if (!frame_tree_root_.get())
    return NULL;

  FrameTreeNode* node = NULL;
  std::queue<FrameTreeNode*> queue;
  queue.push(frame_tree_root_.get());

  while (!queue.empty()) {
    node = queue.front();
    queue.pop();
    if (node->frame_id() == frame_id)
      return node;

    for (size_t i = 0; i < node->child_count(); ++i)
      queue.push(node->child_at(i));
  }

  return NULL;
}
