__insert_file_id (struct stat const *st, enum file_id_type type)
{
   file_id *p;
   static file_id *next_slot;

   if (!next_slot)
     next_slot = xmalloc (sizeof *next_slot);
   next_slot->dev = st->st_dev;
   next_slot->ino = st->st_ino;
   next_slot->queued_output = false;
   p = hash_insert (file_id_table, next_slot);
   if (!p)
     xalloc_die ();
   if (p == next_slot)
     next_slot = NULL;
   p->type = type;
   return p;
}
