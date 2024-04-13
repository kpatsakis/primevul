void ChooserContextBase::SetWebsiteSetting(const GURL& requesting_origin,
                                           const GURL& embedding_origin,
                                           std::unique_ptr<base::Value> value) {
  host_content_settings_map_->SetWebsiteSettingDefaultScope(
      requesting_origin, embedding_origin, data_content_settings_type_,
      std::string(), std::move(value));
}
