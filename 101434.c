void ResourceDispatcherHostImpl::CancelRequestsForContext(
    ResourceContext* context) {
  DCHECK_CURRENTLY_ON(BrowserThread::IO);
  DCHECK(context);

  CHECK(ContainsKey(active_resource_contexts_, context));

  typedef std::vector<scoped_ptr<ResourceLoader>> LoaderList;
  LoaderList loaders_to_cancel;

  for (LoaderMap::iterator i = pending_loaders_.begin();
       i != pending_loaders_.end();) {
    ResourceLoader* loader = i->second.get();
    if (loader->GetRequestInfo()->GetContext() == context) {
      loaders_to_cancel.push_back(std::move(i->second));
      IncrementOutstandingRequestsMemory(-1, *loader->GetRequestInfo());
      pending_loaders_.erase(i++);
    } else {
      ++i;
    }
  }

  for (BlockedLoadersMap::iterator i = blocked_loaders_map_.begin();
       i != blocked_loaders_map_.end();) {
    BlockedLoadersList* loaders = i->second.get();
    if (loaders->empty()) {
      ++i;
      continue;
    }
    ResourceRequestInfoImpl* info = loaders->front()->GetRequestInfo();
    if (info->GetContext() == context) {
      scoped_ptr<BlockedLoadersList> deleter(std::move(i->second));
      blocked_loaders_map_.erase(i++);
      for (auto& loader : *loaders) {
        info = loader->GetRequestInfo();
        DCHECK_EQ(context, info->GetContext());
        IncrementOutstandingRequestsMemory(-1, *info);
        loaders_to_cancel.push_back(std::move(loader));
      }
    } else {
      ++i;
    }
  }

#ifndef NDEBUG
  for (const auto& loader : loaders_to_cancel) {
    DCHECK(loader->GetRequestInfo()->IsDownload() ||
           loader->GetRequestInfo()->is_stream() ||
           (loader->GetRequestInfo()->detachable_handler() &&
            loader->GetRequestInfo()->detachable_handler()->is_detached()) ||
           loader->GetRequestInfo()->GetProcessType() == PROCESS_TYPE_BROWSER ||
           loader->is_transferring());
  }
#endif

  loaders_to_cancel.clear();

  if (async_revalidation_manager_) {
    async_revalidation_manager_->CancelAsyncRevalidationsForResourceContext(
        context);
  }

  for (const auto& loader : pending_loaders_) {
    CHECK_NE(loader.second->GetRequestInfo()->GetContext(), context);
  }

  for (const auto& blocked_loaders : blocked_loaders_map_) {
    BlockedLoadersList* loaders = blocked_loaders.second.get();
    if (!loaders->empty()) {
      ResourceRequestInfoImpl* info = loaders->front()->GetRequestInfo();
      CHECK_NE(info->GetContext(), context);
    }
  }
}
