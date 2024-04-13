tristring_hash (gconstpointer key)
{
  const char *p = key;
  guint h = *p;

  if (h)
    {
      for (p += 1; *p != '\0'; p++)
        h = (h << 5) - h + *p;
    }

  /* skip nul and do the next substring */
  for (p += 1; *p != '\0'; p++)
    h = (h << 5) - h + *p;

  /* skip nul again and another substring */
  for (p += 1; *p != '\0'; p++)
    h = (h << 5) - h + *p;
  
  return h;
}
