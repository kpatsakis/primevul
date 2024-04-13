  Ins_SxyTCA( TT_ExecContext  exc )
  {
    FT_Short  AA, BB;

    FT_Byte  opcode = exc->opcode;


    AA = (FT_Short)( ( opcode & 1 ) << 14 );
    BB = (FT_Short)( AA ^ 0x4000 );

    if ( opcode < 4 )
    {
      exc->GS.projVector.x = AA;
      exc->GS.projVector.y = BB;

      exc->GS.dualVector.x = AA;
      exc->GS.dualVector.y = BB;
    }

    if ( ( opcode & 2 ) == 0 )
    {
      exc->GS.freeVector.x = AA;
      exc->GS.freeVector.y = BB;
    }

    Compute_Funcs( exc );
  }
