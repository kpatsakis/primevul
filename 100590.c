void OmniboxViewWin::TextChanged() {
  ScopedFreeze freeze(this, GetTextObjectModel());
  EmphasizeURLComponents();
  model_->OnChanged();
}
