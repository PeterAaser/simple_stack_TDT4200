#ifdef DEBUG
#define d_printf(...) do{ fprintf( stderr, __VA_ARGS__ ); } while( false )
#else
#define d_printf(...) do{ } while ( false )
#endif
