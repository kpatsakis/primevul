UnlockServer(void)
{
  if (nolock) return;

  if (!StillLocking){

  (void) unlink(LockFile);
  }
}
