bool ShouldFillOnAccountSelect() {
  std::string group_name =
      base::FieldTrialList::FindFullName(kFillOnAccountSelectFieldTrialName);

  if (base::CommandLine::ForCurrentProcess()->HasSwitch(
          switches::kDisableFillOnAccountSelect)) {
    return false;
  }

  if (base::CommandLine::ForCurrentProcess()->HasSwitch(
          switches::kEnableFillOnAccountSelect) ||
      base::CommandLine::ForCurrentProcess()->HasSwitch(
          switches::kEnableFillOnAccountSelectNoHighlighting)) {
    return true;
  }

  return group_name ==
             kFillOnAccountSelectFieldTrialEnabledWithHighlightGroup ||
         group_name ==
             kFillOnAccountSelectFieldTrialEnabledWithNoHighlightGroup;
}
