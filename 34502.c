get_kinfo_proc (gint pid,
#ifdef HAVE_NETBSD
                struct kinfo_proc2 *p)
#else
                struct kinfo_proc *p)
#endif
{
  int name[6];
  u_int namelen;
  size_t sz;

  sz = sizeof(*p);
  namelen = 0;
  name[namelen++] = CTL_KERN;
#ifdef HAVE_NETBSD
  name[namelen++] = KERN_PROC2;
#else
  name[namelen++] = KERN_PROC;
#endif
  name[namelen++] = KERN_PROC_PID;
  name[namelen++] = pid;
  name[namelen++] = sz;
  name[namelen++] = 1;

  if (sysctl (name, namelen, p, &sz, NULL, 0) == -1)
    return FALSE;

  return TRUE;
}
