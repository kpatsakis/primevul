struct ebt_entry *ebt_next_entry(const struct ebt_entry *entry)
{
	return (void *)entry + entry->next_offset;
}
