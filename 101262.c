SnapCoordinator* Document::GetSnapCoordinator() {
  if (!snap_coordinator_)
    snap_coordinator_ = SnapCoordinator::Create();

  return snap_coordinator_.Get();
}
