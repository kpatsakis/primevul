void RenderFrameDevToolsAgentHost::GrantPolicy() {
  if (!frame_host_)
    return;
  uint32_t process_id = frame_host_->GetProcess()->GetID();
  if (base::FeatureList::IsEnabled(network::features::kNetworkService))
    GetNetworkService()->SetRawHeadersAccess(process_id, true);
  ChildProcessSecurityPolicyImpl::GetInstance()->GrantReadRawCookies(
      process_id);
}
