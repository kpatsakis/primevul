void CommandBufferProxyImpl::OrderingBarrierHelper(int32_t put_offset) {
  DCHECK(has_buffer_);

  if (last_put_offset_ == put_offset)
    return;
  last_put_offset_ = put_offset;
  last_flush_id_ =
      channel_->OrderingBarrier(route_id_, put_offset, snapshot_requested_,
                                std::move(pending_sync_token_fences_));

  snapshot_requested_ = false;
  pending_sync_token_fences_.clear();

  flushed_fence_sync_release_ = next_fence_sync_release_ - 1;
}
