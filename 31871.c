iter_next (DBusMessageDataIter *iter)
{
  iter->sequence_nos[iter->depth] += 1;
}
