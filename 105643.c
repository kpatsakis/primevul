usage (int argc,
       char *argv[])
{
  execlp ("man", "man", "part_id", NULL);
  g_printerr ("Cannot show man page: %m\n");
  exit (1);
}
