bool TabsGetFunction::RunImpl() {
  int tab_id = -1;
  EXTENSION_FUNCTION_VALIDATE(args_->GetInteger(0, &tab_id));

  TabStripModel* tab_strip = NULL;
  WebContents* contents = NULL;
  int tab_index = -1;
  if (!GetTabById(tab_id, profile(), include_incognito(),
                  NULL, &tab_strip, &contents, &tab_index, &error_))
    return false;

  SetResult(ExtensionTabUtil::CreateTabValue(contents,
                                             tab_strip,
                                             tab_index,
                                             GetExtension()));
  return true;
}
