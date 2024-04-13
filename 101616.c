void RenderFrameDevToolsAgentHost::FrameDeleted(RenderFrameHost* rfh) {
  RenderFrameHostImpl* host = static_cast<RenderFrameHostImpl*>(rfh);
  for (auto* tracing : protocol::TracingHandler::ForAgentHost(this))
    tracing->FrameDeleted(host);
  if (host->frame_tree_node() == frame_tree_node_) {
    DestroyOnRenderFrameGone();
  }
}
