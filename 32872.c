  load_mac_face( FT_Library           library,
                 FT_Stream            stream,
                 FT_Long              face_index,
                 FT_Face             *aface,
                 const FT_Open_Args  *args )
  {
    FT_Error error;
    FT_UNUSED( args );


    error = IsMacBinary( library, stream, face_index, aface );
    if ( FT_ERROR_BASE( error ) == FT_Err_Unknown_File_Format )
    {

#undef  FT_COMPONENT
#define FT_COMPONENT  trace_raccess

      FT_TRACE3(( "Try as dfont: %s ...", args->pathname ));

      error = IsMacResource( library, stream, 0, face_index, aface );

      FT_TRACE3(( "%s\n", error ? "failed" : "successful" ));

#undef  FT_COMPONENT
#define FT_COMPONENT  trace_objs

    }

    if ( ( FT_ERROR_BASE( error ) == FT_Err_Unknown_File_Format      ||
           FT_ERROR_BASE( error ) == FT_Err_Invalid_Stream_Operation ) &&
         ( args->flags & FT_OPEN_PATHNAME )                            )
      error = load_face_in_embedded_rfork( library, stream,
                                           face_index, aface, args );
    return error;
  }
