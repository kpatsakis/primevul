void OmniboxViewViews::Update() {
  const security_state::SecurityLevel old_security_level = security_level_;
  UpdateSecurityLevel();
  if (model()->UpdatePermanentText()) {
    RevertAll();

    if (model()->has_focus())
      SelectAll(true);
  } else if (old_security_level != security_level_) {
    EmphasizeURLComponents();
  }
}
