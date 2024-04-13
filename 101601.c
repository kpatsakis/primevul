void UpdatePolicyDuration(base::TimeTicks now,
                          base::TimeTicks policy_expiration,
                          base::TimeDelta* policy_duration) {
  if (policy_expiration <= now)
    return;

  if (policy_duration->is_zero()) {
    *policy_duration = policy_expiration - now;
    return;
  }

  *policy_duration = std::min(*policy_duration, policy_expiration - now);
}
