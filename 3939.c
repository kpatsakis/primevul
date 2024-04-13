bool WddxPacket::recursiveAddVar(const String& varName,
                                 const Variant& varVariant,
                                 bool hasVarTag) {
  SeenContainers seen;
  return recursiveAddVarImpl(varName, varVariant, hasVarTag, seen);
}