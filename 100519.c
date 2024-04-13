RootWindow::~RootWindow() {
  if (compositor_lock_) {
    draw_on_compositor_unlock_ = false;
    compositor_lock_->CancelLock();
    DCHECK(!compositor_lock_);
  }
  compositor_->RemoveObserver(this);
  compositor_.reset();

  host_.reset(NULL);

  layer()->GetAnimator()->RemoveObserver(this);
}
