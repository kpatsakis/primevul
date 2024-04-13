static boolean parse_float( const char **pcur, float *val )
{
   const char *cur = *pcur;
   boolean integral_part = FALSE;
   boolean fractional_part = FALSE;

   if (*cur == '0' && *(cur + 1) == 'x') {
      union fi fi;
      fi.ui = strtoul(cur, NULL, 16);
      *val = fi.f;
      cur += 10;
      goto out;
   }

   *val = (float) atof( cur );
   if (*cur == '-' || *cur == '+')
      cur++;
   if (is_digit( cur )) {
      cur++;
      integral_part = TRUE;
      while (is_digit( cur ))
         cur++;
   }
   if (*cur == '.') {
      cur++;
      if (is_digit( cur )) {
         cur++;
         fractional_part = TRUE;
         while (is_digit( cur ))
            cur++;
      }
   }
   if (!integral_part && !fractional_part)
      return FALSE;
   if (uprcase( *cur ) == 'E') {
      cur++;
      if (*cur == '-' || *cur == '+')
         cur++;
      if (is_digit( cur )) {
         cur++;
         while (is_digit( cur ))
            cur++;
      }
      else
         return FALSE;
   }

out:
   *pcur = cur;
   return TRUE;
}
