bool WindowsCreateFunction::RunImpl() {
  DictionaryValue* args = NULL;
  std::vector<GURL> urls;
  WebContents* contents = NULL;

  if (HasOptionalArgument(0))
    EXTENSION_FUNCTION_VALIDATE(args_->GetDictionary(0, &args));

  if (args) {
    if (args->HasKey(keys::kUrlKey)) {
      Value* url_value;
      std::vector<std::string> url_strings;
      args->Get(keys::kUrlKey, &url_value);

      if (url_value->IsType(Value::TYPE_STRING)) {
        std::string url_string;
        url_value->GetAsString(&url_string);
        url_strings.push_back(url_string);
      } else if (url_value->IsType(Value::TYPE_LIST)) {
        const ListValue* url_list = static_cast<const ListValue*>(url_value);
        for (size_t i = 0; i < url_list->GetSize(); ++i) {
          std::string url_string;
          EXTENSION_FUNCTION_VALIDATE(url_list->GetString(i, &url_string));
          url_strings.push_back(url_string);
        }
      }

      for (std::vector<std::string>::iterator i = url_strings.begin();
           i != url_strings.end(); ++i) {
        GURL url = ExtensionTabUtil::ResolvePossiblyRelativeURL(
            *i, GetExtension());
        if (!url.is_valid()) {
          error_ = ErrorUtils::FormatErrorMessage(
              keys::kInvalidUrlError, *i);
          return false;
        }
        if (ExtensionTabUtil::IsCrashURL(url)) {
          error_ = keys::kNoCrashBrowserError;
          return false;
        }
        urls.push_back(url);
      }
    }
  }

  if (args) {
    int tab_id = -1;
    if (args->HasKey(keys::kTabIdKey)) {
      EXTENSION_FUNCTION_VALIDATE(args->GetInteger(keys::kTabIdKey, &tab_id));

      TabStripModel* source_tab_strip = NULL;
      int tab_index = -1;
      if (!GetTabById(tab_id, profile(), include_incognito(),
                      NULL, &source_tab_strip,
                      NULL, &tab_index, &error_))
        return false;
      contents = source_tab_strip->DetachWebContentsAt(tab_index);
      if (!contents) {
        error_ = ErrorUtils::FormatErrorMessage(
            keys::kTabNotFoundError, base::IntToString(tab_id));
        return false;
      }
    }
  }

  Profile* window_profile = profile();
  Browser::Type window_type = Browser::TYPE_TABBED;
  bool create_panel = false;

  PanelManager::CreateMode panel_create_mode = PanelManager::CREATE_AS_DOCKED;

  gfx::Rect window_bounds;
  bool focused = true;
  bool saw_focus_key = false;
  std::string extension_id;

  bool is_error = true;
  bool open_incognito_window = ShouldOpenIncognitoWindow(args, &urls,
                                                         &is_error);
  if (is_error) {
    return false;
  }
  if (open_incognito_window) {
    window_profile = window_profile->GetOffTheRecordProfile();
  }

  if (args) {
    std::string type_str;
    if (args->HasKey(keys::kWindowTypeKey)) {
      EXTENSION_FUNCTION_VALIDATE(args->GetString(keys::kWindowTypeKey,
                                                  &type_str));
      if (type_str == keys::kWindowTypeValuePopup) {
        window_type = Browser::TYPE_POPUP;
        extension_id = GetExtension()->id();
      } else if (type_str == keys::kWindowTypeValuePanel ||
                 type_str == keys::kWindowTypeValueDetachedPanel) {
        extension_id = GetExtension()->id();
        bool use_panels = false;
#if !defined(OS_ANDROID)
        use_panels = PanelManager::ShouldUsePanels(extension_id);
#endif
        if (use_panels) {
          create_panel = true;
#if !defined(OS_CHROMEOS)
          if (type_str == keys::kWindowTypeValueDetachedPanel)
            panel_create_mode = PanelManager::CREATE_AS_DETACHED;
#endif
        } else {
          window_type = Browser::TYPE_POPUP;
        }
      } else if (type_str != keys::kWindowTypeValueNormal) {
        error_ = keys::kInvalidWindowTypeError;
        return false;
      }
    }

    if (window_type == Browser::TYPE_TABBED ||
        window_type == Browser::TYPE_POPUP ||
        create_panel) {
      ui::WindowShowState show_state = ui::SHOW_STATE_DEFAULT;
      WindowSizer::GetBrowserWindowBoundsAndShowState(std::string(),
                                                      gfx::Rect(),
                                                      GetCurrentBrowser(),
                                                      &window_bounds,
                                                      &show_state);
    }

    if (create_panel && PanelManager::CREATE_AS_DETACHED == panel_create_mode) {
      window_bounds.set_origin(
          PanelManager::GetInstance()->GetDefaultDetachedPanelOrigin());
    }

    int bounds_val = -1;
    if (args->HasKey(keys::kLeftKey)) {
      EXTENSION_FUNCTION_VALIDATE(args->GetInteger(keys::kLeftKey,
                                                   &bounds_val));
      window_bounds.set_x(bounds_val);
    }

    if (args->HasKey(keys::kTopKey)) {
      EXTENSION_FUNCTION_VALIDATE(args->GetInteger(keys::kTopKey,
                                                   &bounds_val));
      window_bounds.set_y(bounds_val);
    }

    if (args->HasKey(keys::kWidthKey)) {
      EXTENSION_FUNCTION_VALIDATE(args->GetInteger(keys::kWidthKey,
                                                   &bounds_val));
      window_bounds.set_width(bounds_val);
    }

    if (args->HasKey(keys::kHeightKey)) {
      EXTENSION_FUNCTION_VALIDATE(args->GetInteger(keys::kHeightKey,
                                                   &bounds_val));
      window_bounds.set_height(bounds_val);
    }

    if (args->HasKey(keys::kFocusedKey)) {
      EXTENSION_FUNCTION_VALIDATE(args->GetBoolean(keys::kFocusedKey,
                                                   &focused));
      saw_focus_key = true;
    }
  }

  if (create_panel) {
    if (urls.empty())
      urls.push_back(GURL(chrome::kChromeUINewTabURL));

#if defined(OS_CHROMEOS)
    if (PanelManager::ShouldUsePanels(extension_id)) {
      ShellWindow::CreateParams create_params;
      create_params.window_type = ShellWindow::WINDOW_TYPE_V1_PANEL;
      create_params.bounds = window_bounds;
      create_params.focused = saw_focus_key && focused;
      ShellWindow* shell_window =
          new ShellWindow(window_profile, GetExtension());
      AshPanelContents* ash_panel_contents = new AshPanelContents(shell_window);
      shell_window->Init(urls[0], ash_panel_contents, create_params);
      SetResult(ash_panel_contents->GetExtensionWindowController()->
                CreateWindowValueWithTabs(GetExtension()));
      extensions::ShellWindowRegistry::Get(window_profile)->AddShellWindow(
          shell_window);
      return true;
    }
#else
    std::string title =
        web_app::GenerateApplicationNameFromExtensionId(extension_id);
    Panel* panel = PanelManager::GetInstance()->CreatePanel(
        title, window_profile, urls[0], window_bounds, panel_create_mode);

    if (!saw_focus_key || !focused)
      panel->ShowInactive();
    else
      panel->Show();

    SetResult(
        panel->extension_window_controller()->CreateWindowValueWithTabs(
            GetExtension()));
    return true;
#endif
  }

  chrome::HostDesktopType host_desktop_type = chrome::GetActiveDesktop();
  if (create_panel)
    window_type = Browser::TYPE_POPUP;
  Browser::CreateParams create_params(window_type, window_profile,
                                      host_desktop_type);
  if (extension_id.empty()) {
    create_params.initial_bounds = window_bounds;
  } else {
    create_params = Browser::CreateParams::CreateForApp(
        window_type,
        web_app::GenerateApplicationNameFromExtensionId(extension_id),
        window_bounds,
        window_profile,
        host_desktop_type);
  }
  create_params.initial_show_state = ui::SHOW_STATE_NORMAL;
  create_params.host_desktop_type = chrome::GetActiveDesktop();

  Browser* new_window = CreateBrowserWindow(create_params, window_profile,
                                            extension_id);

  for (std::vector<GURL>::iterator i = urls.begin(); i != urls.end(); ++i) {
    WebContents* tab = chrome::AddSelectedTabWithURL(
        new_window, *i, content::PAGE_TRANSITION_LINK);
    if (create_panel) {
      extensions::TabHelper::FromWebContents(tab)->
          SetExtensionAppIconById(extension_id);
    }
  }
  if (contents) {
    TabStripModel* target_tab_strip = new_window->tab_strip_model();
    target_tab_strip->InsertWebContentsAt(urls.size(), contents,
                                          TabStripModel::ADD_NONE);
  } else if (urls.empty() && window_type != Browser::TYPE_POPUP) {
    chrome::NewTab(new_window);
  }
  chrome::SelectNumberedTab(new_window, 0);

  if (!saw_focus_key && create_panel)
    focused = false;

  if (focused)
    new_window->window()->Show();
  else
    new_window->window()->ShowInactive();

  if (new_window->profile()->IsOffTheRecord() && !include_incognito()) {
    SetResult(Value::CreateNullValue());
  } else {
    SetResult(
        new_window->extension_window_controller()->CreateWindowValueWithTabs(
            GetExtension()));
  }

  return true;
}
