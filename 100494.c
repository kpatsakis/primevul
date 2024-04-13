void PpapiPluginProcessHost::OnProcessLaunched() {
   host_impl_->set_plugin_process_handle(process_->GetHandle());
 }
