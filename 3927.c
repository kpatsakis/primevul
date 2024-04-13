bool WddxPacket::add_var(const String& varName, bool hasVarTag) {
  VarEnv* v = g_context->getOrCreateVarEnv();
  if (!v) return false;
  Variant varVariant = *reinterpret_cast<Variant*>(v->lookup(varName.get()));
  return recursiveAddVar(varName, varVariant, hasVarTag);
}