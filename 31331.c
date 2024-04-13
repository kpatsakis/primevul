tristring_alloc_from_strings (size_t      padding_before,
                              const char *name,
                              const char *path,
                              const char *interface)
{
  size_t name_len, iface_len, path_len, len;
  char *tri;
  
  if (name)
    name_len = strlen (name);
  else
    name_len = 0;

  path_len = strlen (path);
  
  iface_len = strlen (interface);

  tri = g_malloc (padding_before + name_len + path_len + iface_len + 3);

  len = padding_before;
  
  if (name)
    memcpy (&tri[len], name, name_len);

  len += name_len;
  tri[len] = '\0';
  len += 1;

  g_assert (len == (padding_before + name_len + 1));
  
  memcpy (&tri[len], path, path_len);
  len += path_len;
  tri[len] = '\0';
  len += 1;

  g_assert (len == (padding_before + name_len + path_len + 2));
  
  memcpy (&tri[len], interface, iface_len);
  len += iface_len;
  tri[len] = '\0';
  len += 1;

  g_assert (len == (padding_before + name_len + path_len + iface_len + 3));

  return tri;
}
