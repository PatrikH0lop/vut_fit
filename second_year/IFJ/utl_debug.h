/**
 * Projekt IFJ 2017
 * @brief Rozhrani pro ladeni
 * @Author HanakJiri, xhanak33
 * @Author Patrik Holop, xholop01
 * @file utl_debug.h
 */

#ifndef IFJ17_FIT_2017_UTL_DEBUG_H
#define IFJ17_FIT_2017_UTL_DEBUG_H


#define DEBUG_SCANNER 0
#define DEBUG_PARSER 0
#define DEBUG_SEMANTICS 0
#define DEBUG_EXPR_SEMANTICS 0
#define DEBUG_EXPR_GENERATE 0
#define DEBUG_SYMTABLE 0
#define DEBUG_FN_PRINT 0
#define DEBUG_GENERATOR 0
//#define DEBUG_CRASH 0

#define TEST_PARSER 0


#ifndef DEBUG_PARSER
#define dbg_msg(...) fprintf(stderr, __VA_ARGS__ )
#define dbg_print_stack(s) stack_print(s)
#else
#define dbg_msg(...) (0)
#define dbg_print_stack(s) (0)
#endif

#ifndef TEST_PARSER
#define sya_test_msg(...) fprintf(stderr, __VA_ARGS__ )
#else
#define sya_test_msg(...) (0)
#endif

#ifndef DEBUG_SCANNER
#define dbg_scan_msg(...) fprintf(stderr, __VA_ARGS__ )
#else
#define dbg_scan_msg(...) (0)
#endif

#ifndef DEBUG_SEMANTICS
#define sea_dbg_msg(...) fprintf(stderr, __VA_ARGS__);
#else
#define sea_dbg_msg(...) (0)
#endif

#ifndef DEBUG_EXPR_GENERATE
#define sea_dbg_expr_gen_msg(...) fprintf(stderr, __VA_ARGS__);
#else
#define sea_dbg_expr_gen_msg(...) (0)
#endif

#ifndef DEBUG_EXPR_SEMANTICS
#define sea_dbg_expr_msg(...) fprintf(stderr, __VA_ARGS__);
#else
#define sea_dbg_expr_msg(...) (0)
#endif

#ifndef DEBUG_GENERATOR
#define dbg_gen_msg(...) fprintf(g->f,"#" __VA_ARGS__ "\n")
#else
#define dbg_gen_msg(...) (0)
#endif

#ifndef DEBUG_FN_PRINT
#define dbg_fn_msg(...) fprintf(stderr, __VA_ARGS__ )
#else
#define dbg_fn_msg(...) (0)
#endif

#ifndef DEBUG_SYMTABLE
#define dbg_symtable_msg(...) fprintf(stderr, __VA_ARGS__ )
#else
#define dbg_symtable_msg(...) (0)
#endif


#ifndef DEBUG_CRASH
#define dbg_crash_msg(...) fprintf(stderr, "I am crashing!!! "__FILE__ ": %u: " __VA_ARGS__ "\n", __LINE__)
#else
#define dbg_crash_msg(...) (0)
#endif

#endif //IFJ17_FIT_2017_UTL_DEBUG_H
