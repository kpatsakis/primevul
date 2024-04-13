void DispatchToAgents(FrameTreeNode* frame_tree_node,
                      void (Handler::*method)(MethodArgs...),
                      Args&&... args) {
  RenderFrameDevToolsAgentHost* agent_host =
      FindAgentHost(GetFrameTreeNodeAncestor(frame_tree_node));
  if (!agent_host)
    return;
  for (auto* h : Handler::ForAgentHost(agent_host))
    (h->*method)(std::forward<Args>(args)...);
}
