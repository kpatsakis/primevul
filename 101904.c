void DevToolsWindow::CreateDevToolsBrowser() {
  PrefService* prefs = profile_->GetPrefs();
  if (!prefs->GetDictionary(prefs::kAppWindowPlacement)->HasKey(kDevToolsApp)) {
    DictionaryPrefUpdate update(prefs, prefs::kAppWindowPlacement);
    base::DictionaryValue* wp_prefs = update.Get();
    auto dev_tools_defaults = base::MakeUnique<base::DictionaryValue>();
    dev_tools_defaults->SetInteger("left", 100);
    dev_tools_defaults->SetInteger("top", 100);
    dev_tools_defaults->SetInteger("right", 740);
    dev_tools_defaults->SetInteger("bottom", 740);
    dev_tools_defaults->SetBoolean("maximized", false);
    dev_tools_defaults->SetBoolean("always_on_top", false);
    wp_prefs->Set(kDevToolsApp, std::move(dev_tools_defaults));
  }

  browser_ = new Browser(Browser::CreateParams::CreateForDevTools(profile_));
  browser_->tab_strip_model()->AddWebContents(
      main_web_contents_, -1, ui::PAGE_TRANSITION_AUTO_TOPLEVEL,
      TabStripModel::ADD_ACTIVE);
  main_web_contents_->GetRenderViewHost()->SyncRendererPrefs();
}
