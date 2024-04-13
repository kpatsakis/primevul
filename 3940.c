String WddxPacket::wrapValue(const String& start,
                             const String& end,
                             const String& varValue,
                             const String& varName,
                             bool hasVarTag) {
  StringBuffer valueStr;

  if (hasVarTag) {
    valueStr.append("<var name='");
    valueStr.append(varName);
    valueStr.append("'>");
  }

  valueStr.append(start);
  valueStr.append(varValue);
  valueStr.append(end);

  if (hasVarTag) {
    valueStr.append("</var>");
  }

  return valueStr.detach();
}