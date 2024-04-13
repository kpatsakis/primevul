const SecurityOrigin* BaseAudioContext::GetSecurityOrigin() const {
  if (GetExecutionContext())
    return GetExecutionContext()->GetSecurityOrigin();

  return nullptr;
}
