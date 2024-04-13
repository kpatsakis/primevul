count_entries (PartitionTable *pt)
{
  guint ret;
  guint num_top_level;
  guint n;

  ret = 0;

  num_top_level = part_table_get_num_entries (pt);
  for (n = 0; n < num_top_level; n++)
    {
      PartitionTable *nested;

      if (part_table_entry_is_in_use (pt, n))
        ret++;

      nested = part_table_entry_get_nested (pt, n);
      if (nested != NULL)
        {
          ret += part_table_get_num_entries (nested);
        }
    }

  return ret;
}
