void ExtensionPrefs::AddToExtensionPrefStringSet(
    const std::string& extension_id,
    const std::string& pref_key,
    const std::set<std::string>& added_value) {
  std::set<std::string> old_value;
  std::set<std::string> new_value;
  ReadExtensionPrefStringSet(extension_id, pref_key, &old_value);

  std::set_union(old_value.begin(), old_value.end(),
                 added_value.begin(), added_value.end(),
                 std::inserter(new_value, new_value.begin()));

  ListValue* value = new ListValue();
  for (std::set<std::string>::const_iterator iter = new_value.begin();
       iter != new_value.end(); ++iter)
    value->Append(Value::CreateStringValue(*iter));

  UpdateExtensionPref(extension_id, pref_key, value);
}
