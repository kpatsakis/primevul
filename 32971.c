  GetShortIns( TT_ExecContext  exc )
  {
    /* Reading a byte stream so there is no endianness (DaveP) */
    exc->IP += 2;
    return (FT_Short)( ( exc->code[exc->IP - 2] << 8 ) +
                         exc->code[exc->IP - 1]      );
  }
