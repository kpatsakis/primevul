add_to_eblock(error_block *eblock, uschar *t1, uschar *t2)
{
  error_block *eb = malloc(sizeof(error_block));
  if (eblock == NULL)
    eblock = eb;
  else
  {
    /* Find the end of the eblock struct and point it at eb */
    error_block *tmp = eblock;
    while(tmp->next != NULL)
      tmp = tmp->next;
    tmp->next = eb;
  }
  eb->text1 = t1;
  eb->text2 = t2;
  eb->next  = NULL;
  return eblock;
}
