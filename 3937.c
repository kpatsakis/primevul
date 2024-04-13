String WddxPacket::getWddxEncoded(const String& varType,
                                  const String& varValue,
                                  const String& varName,
                                  bool hasVarTag) {
  if (varType.compare("NULL") == 0 || varType.compare("null") == 0) {
    return wrapValue("<null/>", "", "", varName, hasVarTag);
  }
  if (varType.compare("boolean") == 0) {
    return wrapValue("<boolean value='", "'/>", varValue, varName, hasVarTag);
  }
  if (varType.compare("integer") == 0 || varType.compare("double") == 0) {
    return wrapValue("<number>", "</number>", varValue, varName, hasVarTag);
  }
  if (varType.compare("string") == 0) {
    return wrapValue("<string>", "</string>", varValue, varName, hasVarTag);
  }
  return "";
}