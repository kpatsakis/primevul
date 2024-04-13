bool TabSpecificContentSettings::IsContentAccessed(
    ContentSettingsType content_type) const {
  if (content_type != CONTENT_SETTINGS_TYPE_COOKIES)
    return false;

  return content_accessed_[content_type];
}
