DevToolsUIBindings* DevToolsUIBindings::ForWebContents(
     content::WebContents* web_contents) {
  if (!g_instances.IsCreated())
    return NULL;
  DevToolsUIBindingsList* instances = g_instances.Pointer();
  for (DevToolsUIBindingsList::iterator it(instances->begin());
       it != instances->end(); ++it) {
    if ((*it)->web_contents() == web_contents)
      return *it;
 }
 return NULL;
}
