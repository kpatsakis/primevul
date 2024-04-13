  FT_Create_Class_cff_field_handlers( FT_Library           library,
                                      CFF_Field_Handler**  output_class )
  {
    CFF_Field_Handler*  clazz  = NULL;
    FT_Error            error;
    FT_Memory           memory = library->memory;

    int  i = 0;


#undef CFF_FIELD
#define CFF_FIELD( code, name, id, kind ) i++;
#undef CFF_FIELD_DELTA
#define CFF_FIELD_DELTA( code, name, max, id ) i++;
#undef CFF_FIELD_CALLBACK
#define CFF_FIELD_CALLBACK( code, name, id ) i++;

#include "cfftoken.h"

    i++; /* { 0, 0, 0, 0, 0, 0, 0 } */

    if ( FT_ALLOC( clazz, sizeof ( CFF_Field_Handler ) * i ) )
      return error;

    i = 0;


#ifndef FT_DEBUG_LEVEL_TRACE


#undef CFF_FIELD_CALLBACK
#define CFF_FIELD_CALLBACK( code_, name_, id_ )        \
          clazz[i].kind         = cff_kind_callback;   \
          clazz[i].code         = code_ | CFFCODE;     \
          clazz[i].offset       = 0;                   \
          clazz[i].size         = 0;                   \
          clazz[i].reader       = cff_parse_ ## name_; \
          clazz[i].array_max    = 0;                   \
          clazz[i].count_offset = 0;                   \
          i++;

#undef  CFF_FIELD
#define CFF_FIELD( code_, name_, id_, kind_ )               \
          clazz[i].kind         = kind_;                    \
          clazz[i].code         = code_ | CFFCODE;          \
          clazz[i].offset       = FT_FIELD_OFFSET( name_ ); \
          clazz[i].size         = FT_FIELD_SIZE( name_ );   \
          clazz[i].reader       = 0;                        \
          clazz[i].array_max    = 0;                        \
          clazz[i].count_offset = 0;                        \
          i++;                                              \

#undef  CFF_FIELD_DELTA
#define CFF_FIELD_DELTA( code_, name_, max_, id_ )                  \
          clazz[i].kind         = cff_kind_delta;                   \
          clazz[i].code         = code_ | CFFCODE;                  \
          clazz[i].offset       = FT_FIELD_OFFSET( name_ );         \
          clazz[i].size         = FT_FIELD_SIZE_DELTA( name_ );     \
          clazz[i].reader       = 0;                                \
          clazz[i].array_max    = max_;                             \
          clazz[i].count_offset = FT_FIELD_OFFSET( num_ ## name_ ); \
          i++;

#include "cfftoken.h"

    clazz[i].kind         = 0;
    clazz[i].code         = 0;
    clazz[i].offset       = 0;
    clazz[i].size         = 0;
    clazz[i].reader       = 0;
    clazz[i].array_max    = 0;
    clazz[i].count_offset = 0;


#else /* FT_DEBUG_LEVEL_TRACE */


#undef CFF_FIELD_CALLBACK
#define CFF_FIELD_CALLBACK( code_, name_, id_ )        \
          clazz[i].kind         = cff_kind_callback;   \
          clazz[i].code         = code_ | CFFCODE;     \
          clazz[i].offset       = 0;                   \
          clazz[i].size         = 0;                   \
          clazz[i].reader       = cff_parse_ ## name_; \
          clazz[i].array_max    = 0;                   \
          clazz[i].count_offset = 0;                   \
          clazz[i].id           = id_;                 \
          i++;

#undef  CFF_FIELD
#define CFF_FIELD( code_, name_, id_, kind_ )               \
          clazz[i].kind         = kind_;                    \
          clazz[i].code         = code_ | CFFCODE;          \
          clazz[i].offset       = FT_FIELD_OFFSET( name_ ); \
          clazz[i].size         = FT_FIELD_SIZE( name_ );   \
          clazz[i].reader       = 0;                        \
          clazz[i].array_max    = 0;                        \
          clazz[i].count_offset = 0;                        \
          clazz[i].id           = id_;                      \
          i++;                                              \

#undef  CFF_FIELD_DELTA
#define CFF_FIELD_DELTA( code_, name_, max_, id_ )                  \
          clazz[i].kind         = cff_kind_delta;                   \
          clazz[i].code         = code_ | CFFCODE;                  \
          clazz[i].offset       = FT_FIELD_OFFSET( name_ );         \
          clazz[i].size         = FT_FIELD_SIZE_DELTA( name_ );     \
          clazz[i].reader       = 0;                                \
          clazz[i].array_max    = max_;                             \
          clazz[i].count_offset = FT_FIELD_OFFSET( num_ ## name_ ); \
          clazz[i].id           = id_;                              \
          i++;

#include "cfftoken.h"

    clazz[i].kind         = 0;
    clazz[i].code         = 0;
    clazz[i].offset       = 0;
    clazz[i].size         = 0;
    clazz[i].reader       = 0;
    clazz[i].array_max    = 0;
    clazz[i].count_offset = 0;
    clazz[i].id           = 0;


#endif /* FT_DEBUG_LEVEL_TRACE */


    *output_class = clazz;

    return FT_Err_Ok;
  }
