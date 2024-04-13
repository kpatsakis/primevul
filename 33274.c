__imlib_DupUpdates(ImlibUpdate * u)
{
   ImlibUpdate        *uu, *cu, *pu, *ru;

   if (!u)
      return NULL;

   uu = malloc(sizeof(ImlibUpdate));
   memcpy(uu, u, sizeof(ImlibUpdate));
   cu = u->next;
   pu = u;
   ru = uu;
   while (cu)
     {
        uu = malloc(sizeof(ImlibUpdate));
        memcpy(uu, u, sizeof(ImlibUpdate));
        pu->next = uu;
        pu = cu;
        cu = cu->next;
     }
   return ru;
}
