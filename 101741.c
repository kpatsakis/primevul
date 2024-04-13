scoped_refptr<SiteInstance> RenderFrameHostManager::ConvertToSiteInstance(
    const SiteInstanceDescriptor& descriptor,
    SiteInstance* candidate_instance) {
  SiteInstanceImpl* current_instance = render_frame_host_->GetSiteInstance();

  if (descriptor.existing_site_instance)
    return descriptor.existing_site_instance;

  if (descriptor.relation == SiteInstanceRelation::RELATED)
    return current_instance->GetRelatedSiteInstance(descriptor.dest_url);

  if (descriptor.relation == SiteInstanceRelation::RELATED_DEFAULT_SUBFRAME)
    return current_instance->GetDefaultSubframeSiteInstance();

  if (candidate_instance &&
      !current_instance->IsRelatedSiteInstance(candidate_instance) &&
      candidate_instance->GetSiteURL() ==
          SiteInstance::GetSiteForURL(descriptor.browser_context,
                                      descriptor.dest_url)) {
    return candidate_instance;
  }

  return SiteInstance::CreateForURL(
      delegate_->GetControllerForRenderManager().GetBrowserContext(),
      descriptor.dest_url);
}
