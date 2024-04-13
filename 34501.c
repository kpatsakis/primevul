get_kinfo_proc (pid_t pid, struct kinfo_proc *p)
{
  int mib[4];
  size_t len;

  len = 4;
  sysctlnametomib ("kern.proc.pid", mib, &len);

  len = sizeof (struct kinfo_proc);
  mib[3] = pid;

  if (sysctl (mib, 4, p, &len, NULL, 0) == -1)
    return FALSE;

  return TRUE;
}
