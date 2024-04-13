static char uprcase( char c )
{
   if (c >= 'a' && c <= 'z')
      return c + 'A' - 'a';
   return c;
}
