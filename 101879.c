bool ProfilingProcessHost::OnMemoryDump(
    const base::trace_event::MemoryDumpArgs& args,
    base::trace_event::ProcessMemoryDump* pmd) {
  profiling_service_->DumpProcessesForTracing(
      base::BindOnce(&ProfilingProcessHost::OnDumpProcessesForTracingCallback,
                     base::Unretained(this), args.dump_guid));
  return true;
}
