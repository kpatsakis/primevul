PasswordStore* CredentialManagerImpl::GetPasswordStore() {
  return client_ ? client_->GetPasswordStore() : nullptr;
}
