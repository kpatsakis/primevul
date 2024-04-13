void PasswordAutofillAgent::DidCommitProvisionalLoad(
    bool is_new_navigation, bool is_same_page_navigation) {
  if (is_same_page_navigation) {
    OnSamePageNavigationCompleted();
  }
}
