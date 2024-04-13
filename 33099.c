  SkipCode( TT_ExecContext  exc )
  {
    exc->IP += exc->length;

    if ( exc->IP < exc->codeSize )
    {
      exc->opcode = exc->code[exc->IP];

      exc->length = opcode_length[exc->opcode];
      if ( exc->length < 0 )
      {
        if ( exc->IP + 1 >= exc->codeSize )
          goto Fail_Overflow;
        exc->length = 2 - exc->length * exc->code[exc->IP + 1];
      }

      if ( exc->IP + exc->length <= exc->codeSize )
        return SUCCESS;
    }

  Fail_Overflow:
    exc->error = FT_THROW( Code_Overflow );
    return FAILURE;
  }
