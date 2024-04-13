static void  Free_ContextPos( HB_GPOS_SubTable* st )
{
  HB_ContextPos*   cp = &st->context;

  switch ( cp->PosFormat )
  {
  case 1:  Free_ContextPos1( &cp->cpf.cpf1 ); break;
  case 2:  Free_ContextPos2( &cp->cpf.cpf2 ); break;
  case 3:  Free_ContextPos3( &cp->cpf.cpf3 ); break;
  default:					      break;
  }
}
