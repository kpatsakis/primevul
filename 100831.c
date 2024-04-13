bool TabsGetAllInWindowFunction::RunImpl() {
  int window_id = extension_misc::kCurrentWindowId;
  if (HasOptionalArgument(0))
    EXTENSION_FUNCTION_VALIDATE(args_->GetInteger(0, &window_id));

  Browser* browser = NULL;
  if (!GetBrowserFromWindowID(this, window_id, &browser))
    return false;

  SetResult(ExtensionTabUtil::CreateTabList(browser, GetExtension()));

  return true;
}
