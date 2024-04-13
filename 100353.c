void ExtensionService::Observe(int type,
                               const NotificationSource& source,
                               const NotificationDetails& details) {
  switch (type) {
    case chrome::NOTIFICATION_EXTENSION_PROCESS_TERMINATED: {
      if (profile_ != Source<Profile>(source).ptr()->GetOriginalProfile())
        break;

      ExtensionHost* host = Details<ExtensionHost>(details).ptr();

      MessageLoop::current()->PostTask(
          FROM_HERE,
          method_factory_.NewRunnableMethod(
              &ExtensionService::TrackTerminatedExtension,
              host->extension()));
      break;
    }
    case content::NOTIFICATION_RENDERER_PROCESS_CREATED: {
      RenderProcessHost* process = Source<RenderProcessHost>(source).ptr();
      Profile* host_profile =
          Profile::FromBrowserContext(process->browser_context());
      if (!profile_->IsSameProfile(host_profile->GetOriginalProfile()))
          break;

      std::vector<std::string> function_names;
      ExtensionFunctionDispatcher::GetAllFunctionNames(&function_names);
      process->Send(new ExtensionMsg_SetFunctionNames(function_names));

      process->Send(new ExtensionMsg_SetScriptingWhitelist(
          *Extension::GetScriptingWhitelist()));

      for (size_t i = 0; i < extensions_.size(); ++i) {
        process->Send(new ExtensionMsg_Loaded(
            ExtensionMsg_Loaded_Params(extensions_[i])));
      }
      break;
    }
    case content::NOTIFICATION_RENDERER_PROCESS_TERMINATED: {
      RenderProcessHost* process = Source<RenderProcessHost>(source).ptr();
      Profile* host_profile =
          Profile::FromBrowserContext(process->browser_context());
      if (!profile_->IsSameProfile(host_profile->GetOriginalProfile()))
          break;

      installed_app_hosts_.erase(process->id());
      break;
    }
    case chrome::NOTIFICATION_PREF_CHANGED: {
      std::string* pref_name = Details<std::string>(details).ptr();
      if (*pref_name == prefs::kExtensionInstallAllowList ||
          *pref_name == prefs::kExtensionInstallDenyList) {
        CheckAdminBlacklist();
      } else {
        NOTREACHED() << "Unexpected preference name.";
      }
      break;
    }

    default:
      NOTREACHED() << "Unexpected notification type.";
  }
}
