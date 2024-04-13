void RenderProcessHostImpl::UnregisterAecDumpConsumerOnUIThread(int id) {
  DCHECK_CURRENTLY_ON(BrowserThread::UI);
  auto it =
      std::find(aec_dump_consumers_.begin(), aec_dump_consumers_.end(), id);
  if (it != aec_dump_consumers_.end())
    aec_dump_consumers_.erase(it);
}
