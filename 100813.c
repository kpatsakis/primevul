void DevToolsWindow::DoAction() {
  UpdateFrontendDockSide();
  switch (action_on_load_.type()) {
    case DevToolsToggleAction::kShowConsole:
      CallClientFunction("InspectorFrontendAPI.showConsole", NULL, NULL, NULL);
      break;

    case DevToolsToggleAction::kInspect:
      CallClientFunction("InspectorFrontendAPI.enterInspectElementMode", NULL,
                         NULL, NULL);
      break;

    case DevToolsToggleAction::kShow:
    case DevToolsToggleAction::kToggle:
      break;

    case DevToolsToggleAction::kReveal: {
      const DevToolsToggleAction::RevealParams* params =
          action_on_load_.params();
      CHECK(params);
      base::StringValue url_value(params->url);
      base::FundamentalValue line_value(static_cast<int>(params->line_number));
      base::FundamentalValue column_value(
          static_cast<int>(params->column_number));
      CallClientFunction("InspectorFrontendAPI.revealSourceLine",
                         &url_value,
                         &line_value,
                         &column_value);
      break;
    }
    default:
      NOTREACHED();
      break;
  }
  action_on_load_ = DevToolsToggleAction::Show();
}
