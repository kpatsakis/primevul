PpapiPluginProcessHost::PpapiPluginProcessHost(
    const content::PepperPluginInfo& info,
    const FilePath& profile_data_directory,
    net::HostResolver* host_resolver)
    : network_observer_(new PluginNetworkObserver(this)),
      profile_data_directory_(profile_data_directory),
      is_broker_(false) {
  process_.reset(new BrowserChildProcessHostImpl(
      content::PROCESS_TYPE_PPAPI_PLUGIN, this));

  filter_ = new PepperMessageFilter(PepperMessageFilter::PLUGIN,
                                    host_resolver);

  ppapi::PpapiPermissions permissions(info.permissions);
  host_impl_ = new content::BrowserPpapiHostImpl(this, permissions);

  file_filter_ = new PepperTrustedFileMessageFilter(
      process_->GetData().id, info.name, profile_data_directory);

  process_->GetHost()->AddFilter(filter_.get());
  process_->GetHost()->AddFilter(file_filter_.get());
  process_->GetHost()->AddFilter(host_impl_.get());

  content::GetContentClient()->browser()->DidCreatePpapiPlugin(host_impl_);
}
