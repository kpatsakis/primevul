  FT_New_Face_From_Suitcase( FT_Library    library,
                             const UInt8*  pathname,
                             FT_Long       face_index,
                             FT_Face*      aface )
  {
    FT_Error       error = FT_ERR( Cannot_Open_Resource );
    ResFileRefNum  res_ref;
    ResourceIndex  res_index;
    Handle         fond;
    short          num_faces_in_res;


    if ( noErr != FT_FSPathMakeRes( pathname, &res_ref ) )
      return FT_THROW( Cannot_Open_Resource );

    UseResFile( res_ref );
    if ( ResError() )
      return FT_THROW( Cannot_Open_Resource );

    num_faces_in_res = 0;
    for ( res_index = 1; ; ++res_index )
    {
      short  num_faces_in_fond;


      fond = Get1IndResource( TTAG_FOND, res_index );
      if ( ResError() )
        break;

      num_faces_in_fond  = count_faces( fond, pathname );
      num_faces_in_res  += num_faces_in_fond;

      if ( 0 <= face_index && face_index < num_faces_in_fond && error )
        error = FT_New_Face_From_FOND( library, fond, face_index, aface );

      face_index -= num_faces_in_fond;
    }

    CloseResFile( res_ref );
    if ( !error && aface && *aface )
      (*aface)->num_faces = num_faces_in_res;
    return error;
  }
