put_stringbuf_mem_skip (struct stringbuf *sb, const char *text, size_t n,
                        int skip)
{
  char *p;

  if (!skip)
    {
      put_stringbuf_mem (sb, text, n);
      return;
    }
  if (sb->out_of_core)
    return;

  if (sb->len + n >= sb->size)
    {
      /* Note: we allocate too much here, but we don't care. */
      sb->size += n + 100;
      p = xtryrealloc (sb->buf, sb->size);
      if ( !p)
        {
          sb->out_of_core = 1;
          return;
        }
      sb->buf = p;
    }
  p = sb->buf+sb->len;
  while (n > skip)
    {
      text += skip;
      n -= skip;
      *p++ = *text++;
      n--;
      sb->len++;
    }
}
