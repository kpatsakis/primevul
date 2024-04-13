bool WddxPacket::recursiveAddVarImpl(const String& varName,
                                     const Variant& varVariant,
                                     bool hasVarTag,
                                     SeenContainers& seen) {
  bool isArray = varVariant.isArray();
  bool isObject = varVariant.isObject();

  if (isArray || isObject) {
    Array varAsArray;
    Object varAsObject;
    ArrayOrObject ptr;
    if (isArray) {
      varAsArray = varVariant.toArray();
      ptr = varAsArray.get();
    }
    if (isObject) {
      varAsObject = varVariant.toObject();
      varAsArray = varAsObject.toArray();
      ptr = varAsObject.get();
    }
    assert(!ptr.isNull());
    if (!seen.emplace(ptr).second) {
      raise_warning("recursion detected");
      return false;
    }
    SCOPE_EXIT { seen.erase(ptr); };

    if (hasVarTag) {
      m_packetString.append("<var name='");
      m_packetString.append(varName.data());
      m_packetString.append("'>");
    }

    int length = varAsArray.length();
    if (length > 0) {
      ArrayIter it = ArrayIter(varAsArray);
      if (it.first().isString()) isObject = true;
      if (isObject) {
        m_packetString.append("<struct>");
        if (!isArray) {
          m_packetString.append("<var name='php_class_name'><string>");
          m_packetString.append(varAsObject->getClassName());
          m_packetString.append("</string></var>");
        }
      } else {
        m_packetString.append("<array length='");
        m_packetString.append(std::to_string(length));
        m_packetString.append("'>");
      }
      for (ArrayIter it(varAsArray); it; ++it) {
        auto key = it.first();
        auto const& value = it.secondRef();
        recursiveAddVarImpl(key.toString(), value, isObject, seen);
      }
      if (isObject) {
        m_packetString.append("</struct>");
      }
      else {
        m_packetString.append("</array>");
      }
    }
    else {
      //empty object
      if (isObject) {
        m_packetString.append("<struct>");
        if (!isArray) {
          m_packetString.append("<var name='php_class_name'><string>");
          m_packetString.append(varAsObject->getClassName());
          m_packetString.append("</string></var>");
        }
        m_packetString.append("</struct>");
      }
    }
    if (hasVarTag) {
      m_packetString.append("</var>");
    }
    return true;
  }

  String varType = getDataTypeString(varVariant.getType());
  if (!getWddxEncoded(varType, "", varName, false).empty()) {
    String varValue;
    if (varType.compare("boolean") == 0) {
      varValue = varVariant.toBoolean() ? "true" : "false";
    } else {
      varValue = StringUtil::HtmlEncode(varVariant.toString(),
                                        StringUtil::QuoteStyle::Double,
                                        "UTF-8", false, false).toCppString();
    }
    m_packetString.append(
      getWddxEncoded(varType, varValue, varName, hasVarTag));
    return true;
  }

  return false;
}