ExtensionDevToolsClientHost::ExtensionDevToolsClientHost(
    Profile* profile,
    DevToolsAgentHost* agent_host,
    scoped_refptr<const Extension> extension,
    const Debuggee& debuggee)
    : profile_(profile),
      agent_host_(agent_host),
      extension_(std::move(extension)),
      last_request_id_(0),
      infobar_(nullptr),
      detach_reason_(api::debugger::DETACH_REASON_TARGET_CLOSED),
      extension_registry_observer_(this) {
  CopyDebuggee(&debuggee_, debuggee);

  g_attached_client_hosts.Get().insert(this);

  extension_registry_observer_.Add(ExtensionRegistry::Get(profile_));

  registrar_.Add(this, chrome::NOTIFICATION_APP_TERMINATING,
                 content::NotificationService::AllSources());
}
