static void eat_until_eol( const char **pcur )
{
   while (**pcur != '\0' && **pcur != '\n')
      (*pcur)++;
}
