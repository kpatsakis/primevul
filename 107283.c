inf_gtk_certificate_manager_memrchr(void* buf,
                                    char c,
                                    size_t len)
{
  char* pos;
  char* end;

  pos = buf + len;
  end = buf;

  while(pos >= end)
  {
    if(*(pos - 1) == c)
      return pos - 1;
    --pos;
  }

  return NULL;
}
