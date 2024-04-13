void Document::ClearAXObjectCache() {
  DCHECK_EQ(&AXObjectCacheOwner(), this);

  if (ax_object_cache_)
    ax_object_cache_->Dispose();
  ax_object_cache_.Clear();

  if (ax_contexts_.size() > 0 && GetLayoutView())
    ax_object_cache_ = AXObjectCache::Create(*this);
}
