DevToolsDockSide DevToolsWindow::GetDockSideFromPrefs(Profile* profile) {
  std::string dock_side =
      profile->GetPrefs()->GetString(prefs::kDevToolsDockSide);

  const char kOldPrefBottom[] = "bottom";
  const char kOldPrefRight[] = "right";
  if ((dock_side == kOldPrefBottom) || (dock_side == kOldPrefRight)) {
    if (!profile->GetPrefs()->GetBoolean(prefs::kDevToolsOpenDocked))
      return DEVTOOLS_DOCK_SIDE_UNDOCKED;
    return (dock_side == kOldPrefBottom) ?
        DEVTOOLS_DOCK_SIDE_BOTTOM : DEVTOOLS_DOCK_SIDE_RIGHT;
  }

  if (dock_side == kPrefUndocked)
    return DEVTOOLS_DOCK_SIDE_UNDOCKED;
  if (dock_side == kPrefRight)
    return DEVTOOLS_DOCK_SIDE_RIGHT;
  return DEVTOOLS_DOCK_SIDE_BOTTOM;
}
