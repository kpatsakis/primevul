void AddChangePasswordStrings(content::WebUIDataSource* html_source) {
#if defined(SAFE_BROWSING_DB_LOCAL)
  bool show_softer_warning =
      safe_browsing::PasswordProtectionService::ShouldShowSofterWarning();

  auto title_string_id = show_softer_warning
                             ? IDS_SETTINGS_CHANGE_PASSWORD_TITLE_SOFTER
                             : IDS_SETTINGS_CHANGE_PASSWORD_TITLE;
  LocalizedString localized_strings[] = {
      {"changePasswordPageTitle", title_string_id},
      {"changePasswordPageDetails", IDS_SETTINGS_CHANGE_PASSWORD_DETAIL},
      {"changePasswordPageButton", IDS_SETTINGS_CHANGE_PASSWORD_BUTTON},
  };

  AddLocalizedStringsBulk(html_source, localized_strings,
                          arraysize(localized_strings));

  const std::string icon_id =
      show_softer_warning ? "settings:security" : "cr:warning";
  html_source->AddString("changePasswordPageIcon", icon_id);
#endif
}
