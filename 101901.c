void DelegatedFrameHost::OnCompositingDidCommit(ui::Compositor* compositor) {
  DCHECK(!resize_lock_ || !create_resize_lock_after_commit_);

  if (resize_lock_ &&
      resize_lock_->expected_size() == current_frame_size_in_dip_) {
    resize_lock_.reset();
    create_resize_lock_after_commit_ = true;
  }

  if (create_resize_lock_after_commit_) {
    create_resize_lock_after_commit_ = false;
    MaybeCreateResizeLock();
  }
}
