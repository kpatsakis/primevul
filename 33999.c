  cff_parse_maxstack( CFF_Parser  parser )
  {
    /* maxstack operator can only be used in a Top DICT */
    CFF_FontRecDict  dict  = (CFF_FontRecDict)parser->object;
    FT_Byte**        data  = parser->stack;
    FT_Error         error = FT_Err_Ok;


    if ( !dict )
    {
      error = FT_THROW( Invalid_File_Format );
      goto Exit;
    }

    dict->maxstack = (FT_UInt)cff_parse_num( parser, data++ );
    if ( dict->maxstack > CFF2_MAX_STACK )
      dict->maxstack = CFF2_MAX_STACK;
    if ( dict->maxstack < CFF2_DEFAULT_STACK )
      dict->maxstack = CFF2_DEFAULT_STACK;

    FT_TRACE4(( " %d\n", dict->maxstack ));

  Exit:
    return error;
  }
