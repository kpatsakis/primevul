void ProfilingProcessHost::HandleDumpProcessOnIOThread(base::ProcessId pid,
                                                       base::FilePath file_path,
                                                       base::File file,
                                                       std::string trigger_name,
                                                       base::OnceClosure done) {
  mojo::ScopedHandle handle = mojo::WrapPlatformFile(file.TakePlatformFile());
  profiling_service_->DumpProcess(
      pid, std::move(handle), GetMetadataJSONForTrace(),
      base::BindOnce(&ProfilingProcessHost::OnProcessDumpComplete,
                     base::Unretained(this), std::move(file_path),
                     std::move(trigger_name), std::move(done)));
}
