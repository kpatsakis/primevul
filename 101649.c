bool RemoteFrame::PrepareForCommit() {
  DetachChildren();
  return !!GetPage();
}
