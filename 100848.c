  bool OnNatPolicyUpdate(bool nat_traversal_enabled) {
    DCHECK(context_->network_task_runner()->BelongsToCurrentThread());

    if (allow_nat_traversal_ != nat_traversal_enabled) {
      allow_nat_traversal_ = nat_traversal_enabled;
      LOG(INFO) << "Updated NAT policy.";
      return true;
    }
    return false;
  }
