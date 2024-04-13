void ResourceDispatcherHostImpl::RemovePendingLoader(
    const LoaderMap::iterator& iter) {
  ResourceRequestInfoImpl* info = iter->second->GetRequestInfo();

  IncrementOutstandingRequestsMemory(-1, *info);

  pending_loaders_.erase(iter);
}
