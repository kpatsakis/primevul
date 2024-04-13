  tt_face_free_sbit( TT_Face  face )
  {
    FT_Stream  stream = face->root.stream;


    FT_FRAME_RELEASE( face->sbit_table );
    face->sbit_table_size  = 0;
    face->sbit_table_type  = TT_SBIT_TABLE_TYPE_NONE;
    face->sbit_num_strikes = 0;
  }
