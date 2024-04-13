void PasswordAutofillAgent::OnDestruct() {
  receiver_.reset();
  base::ThreadTaskRunnerHandle::Get()->DeleteSoon(FROM_HERE, this);
}
