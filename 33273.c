__imlib_AddUpdate(ImlibUpdate * u, int x, int y, int w, int h)
{
   ImlibUpdate        *nu;

   if ((w < 1) || (h < 1) || ((x + w) < 1) || ((y + h) < 1))
      return u;
   nu = malloc(sizeof(ImlibUpdate));
   nu->x = x;
   nu->y = y;
   nu->w = w;
   nu->h = h;
   nu->next = u;
   return nu;
}
