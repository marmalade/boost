import os
import lex

class LexFileInfo:

    def __init__(self):
        self.__file_name = ""
        self.__namespace = ""
        self.__main_call = ""
        self.__main_define = ""

    @property
    def file_name(self):
        return self.__file_name

    @file_name.setter
    def file_name(self, value):
        self.__file_name = value

    @property
    def namespace(self):
        return self.__namespace

    @namespace.setter
    def namespace(self, value):
        self.__namespace = value

    @property
    def main_call(self):
        return self.__main_call

    @main_call.setter
    def main_call(self, value):
        self.__main_call = value

    @property
    def main_define(self):
        return self.__main_define

    @main_define.setter
    def main_define(self, value):
        self.__main_define = value


class BoostTestConverter:

    def process(self, input_file):

        tokens = ('INCLUDE_SEQUENCE',
                  'INT_MAIN_VOID', 'INT_NEW_MAIN_VOID', 'INT_MAIN_INT_CHAR',
                  'INT_TEST_MAIN_VOID', 'INT_TEST_MAIN_INT_CHAR',
                  'INT_CPP_MAIN_VOID', 'INT_CPP_MAIN_INT_CHAR',
                  'VOID_TEST_MAIN_VOID', 'VOID_TEST_MAIN_INT_CHAR',
                  'VOID_MAIN_VOID', 'VOID_MAIN_INT_CHAR',
                  'VOID_CPP_MAIN_VOID', 'VOID_CPP_MAIN_INT_CHAR',
                  'INIT_UNIT_TEST_SUITE',
                  'BOOST_SYSTEM_THROW_WITH_NAMESPACE', 'BOOST_POSIX_API_INCLUDE', 'BOOST_WINDOWS_API_INCLUDE',
                  'INT_MAIN_INT_CHAR_CONST',
            )

        t_ignore = ' \t'
        tk_arg_name = r'[a-zA-Z0-9_]*'

        tk_skip_one_more = r'[\s]+'
        tk_skip_any = r'[\s]*'
        tk_diez = r'[#]+' + tk_skip_any
        tk_ifdef = tk_diez + r'[i][f][d][e][f]+'
        tk_else = tk_diez + r'[e][l][s][e]+'
        tk_endif = tk_diez + r'[e][n][d][i][f]+'
        tk_int = r'([i][n][t])+'
        tk_void = r'([v][o][i][d])+'
        tk_char = r'([c][h][a][r])+'
        tk_char_const = r'(([c][h][a][r])+' + tk_skip_any + r'([c][o][n][s][t])+)+'

        tk_formal_args_void = r'[(]+' + tk_skip_any + r'[)]+'

        tk_formal_args_int_char_no_arg_names = r'[(]+' + tk_skip_any + tk_int + tk_skip_any + r',+' + tk_skip_any + tk_char + tk_skip_any + r'[*]+ ' + r'[\w\W][^)]*' + r'[)]+'
        tk_formal_args_int_char = r'[(]+' + tk_skip_any + tk_int + tk_skip_any + tk_arg_name + tk_skip_any + r',+' + tk_skip_any + tk_char + tk_skip_any + r'[*]+ ' + tk_arg_name + r'[\w\W][^)]*' + r'[)]+'
        tk_formal_args_int_char = r'((' + tk_formal_args_int_char + r')|(' + tk_formal_args_int_char_no_arg_names + r'))+'

        tk_formal_args_int_char_const_no_arg_names = r'[(]+' + tk_skip_any + tk_int + tk_skip_any + r',+' + tk_skip_any + tk_char_const + tk_skip_any + r'[*]+ ' + r'[\w\W][^)]*' + r'[)]+'
        tk_formal_args_int_char_const = r'[(]+' + tk_skip_any + tk_int + tk_skip_any + tk_arg_name + tk_skip_any + r',+' + tk_skip_any + tk_char_const + tk_skip_any + r'[*]+ ' + tk_arg_name + r'[\w\W][^)]*' + r'[)]+'
        tk_formal_args_int_char_const = r'((' + tk_formal_args_int_char_const + r')|(' + tk_formal_args_int_char_const_no_arg_names + r'))+'

        tk_main = r'([m][a][i][n])+'
        tk_new_main = r'([N][E][W]_[M][A][I][N])+'
        tk_open_block = r'[{]+'
        tk_close_block = r'[}]+'
        tk_namespace = r'([n][a][m][e][s][p][a][c][e])+'
        tk_boost = r'([b][o][o][s][t])+'

        t_INCLUDE_SEQUENCE = tk_diez + r'[i][n][c][l][u][d][e]+' + tk_skip_any + r'[<"]+[\w\W][^\n\r]*[>"]+'

        t_INT_MAIN_VOID = tk_int + tk_skip_any + tk_main + tk_skip_any + tk_formal_args_void
        t_INT_NEW_MAIN_VOID = tk_int + tk_skip_any + tk_new_main + tk_skip_any + tk_formal_args_void
        t_INT_MAIN_INT_CHAR = tk_int + tk_skip_any + tk_main + tk_skip_any + tk_formal_args_int_char
        t_INT_TEST_MAIN_VOID = tk_int + tk_skip_any + r'([t][e][s][t][_][m][a][i][n])+' + tk_skip_any + tk_formal_args_void
        t_INT_TEST_MAIN_INT_CHAR = tk_int + tk_skip_any + r'([t][e][s][t][_][m][a][i][n])+' + tk_skip_any + tk_formal_args_int_char
        t_INT_CPP_MAIN_VOID = tk_int + tk_skip_any + r'([c][p][p][_][m][a][i][n])+' + tk_skip_any + tk_formal_args_void
        t_INT_CPP_MAIN_INT_CHAR = tk_int + tk_skip_any + r'([c][p][p][_][m][a][i][n])+' + tk_skip_any + tk_formal_args_int_char
        t_VOID_MAIN_VOID = tk_void + tk_skip_any + tk_main + tk_skip_any + tk_formal_args_void
        t_VOID_MAIN_INT_CHAR = tk_void + tk_skip_any + tk_main + tk_skip_any + tk_formal_args_int_char
        t_VOID_TEST_MAIN_VOID = tk_void + tk_skip_any + r'([t][e][s][t][_][m][a][i][n])+' + tk_skip_any + tk_formal_args_void
        t_VOID_TEST_MAIN_INT_CHAR = tk_void + tk_skip_any + r'([t][e][s][t][_][m][a][i][n])+' + tk_skip_any + tk_formal_args_int_char
        t_VOID_CPP_MAIN_VOID = tk_void + tk_skip_any + r'([c][p][p][_][m][a][i][n])+' + tk_skip_any + tk_formal_args_void
        t_VOID_CPP_MAIN_INT_CHAR = tk_void + tk_skip_any + r'([c][p][p][_][m][a][i][n])+' + tk_skip_any + tk_formal_args_int_char
        t_INT_MAIN_INT_CHAR_CONST = tk_int + tk_skip_any + tk_main + tk_skip_any + tk_formal_args_int_char_const

        tk_init_unit_test_suite = r'[i][n][i][t][_][u][n][i][t][_][t][e][s][t][_][s][u][i][t][e]+'
        test_suite1 = r'[t][e][s][t][_][s][u][i][t][e]+' + tk_skip_any + r'[*]+' + tk_skip_any + tk_init_unit_test_suite + tk_skip_any + tk_formal_args_int_char
        test_suite2 = r'([u][n][i][t][_][t][e][s][t][:][:][t][e][s][t][_][s][u][i][t][e])+' + tk_skip_any + r'[*]+' + tk_skip_any + tk_init_unit_test_suite + tk_skip_any + tk_formal_args_int_char
        t_INIT_UNIT_TEST_SUITE = r'(' + test_suite1 + r')|(' + test_suite2 + r')'

        t_BOOST_SYSTEM_THROW_WITH_NAMESPACE = tk_namespace + tk_skip_one_more + tk_boost + tk_skip_any + tk_open_block + tk_skip_any + tk_namespace + tk_skip_one_more + r'[s][y][s][t][e][m]+' + tk_skip_any + tk_open_block + tk_skip_any + r'[B][O][O][S][T][_][S][Y][S][T][E][M][_][D][E][C][L]+[\s]+[v][o][i][d]+[\s]+[t][h][r][o][w][_][t][e][s][t]' + tk_skip_any + tk_formal_args_void + tk_skip_any + r';' + tk_skip_any + tk_close_block + tk_skip_any + tk_close_block
        t_BOOST_POSIX_API_INCLUDE = tk_ifdef + tk_skip_one_more + r'([B][O][O][S][T][_][P][O][S][I][X][_][A][P][I])+' + tk_skip_one_more + t_INCLUDE_SEQUENCE + tk_skip_one_more + tk_else + tk_skip_one_more + t_INCLUDE_SEQUENCE + tk_skip_one_more + tk_endif
        t_BOOST_WINDOWS_API_INCLUDE = tk_ifdef + tk_skip_one_more + r'([B][O][O][S][T][_][W][I][N][D][O][W][S][_][A][P][I])+' + tk_skip_one_more + t_INCLUDE_SEQUENCE + tk_skip_one_more + tk_endif

        def t_error(t):
            t.lexer.skip(1)

        lexer = lex.lex()

        file = open(input_file, "r")
        stream = file.read()
        file.close()

        stream = stream.replace("/*argc*/", "")
        stream = stream.replace("/*argv*/", "")
        stream = stream.replace("/*ac*/", "")
        stream = stream.replace("/*av*/", "")
        stream = stream.replace("\"boost/test/minimal.hpp\"", "<boost/test/unit_test.hpp>")
        stream = stream.replace("<boost/test/minimal.hpp>", "<boost/test/unit_test.hpp>")
        lexer.input(stream)

        last_include_pos = 0
        include_len = 0
        unit_test_suite_pos = -1
        main_pos = -1
        have_main = False
        have_unit_test_suite = False

        main_call = ""
        main_define = ""

        # Tokenize
        while True:
            tok = lexer.token()
            if not tok: break

            tok_type = tok.type
            tok_pos = tok.lexpos

            if tok_type == 'INCLUDE_SEQUENCE' or tok_type == 'BOOST_POSIX_API_INCLUDE' \
               or tok_type == 'BOOST_WINDOWS_API_INCLUDE' or tok_type == 'BOOST_SYSTEM_THROW_WITH_NAMESPACE':
                if tok_pos > last_include_pos:
                    if ( (main_pos > tok_pos) and have_main) or ( (unit_test_suite_pos > tok_pos) and have_unit_test_suite) or ( (not have_main) and (not have_unit_test_suite) ):
                        last_include_pos = tok_pos
                        include_len = len(tok.value)

            if tok_type == 'INT_MAIN_VOID' or tok_type == 'INT_MAIN_INT_CHAR' or \
               tok_type == 'INT_TEST_MAIN_VOID' or tok_type == 'INT_TEST_MAIN_INT_CHAR' or\
               tok_type == 'INT_CPP_MAIN_VOID' or tok_type == 'INT_CPP_MAIN_INT_CHAR' or\
               tok_type == 'VOID_MAIN_VOID' or tok_type == 'VOID_MAIN_INT_CHAR' or\
               tok_type == 'VOID_TEST_MAIN_VOID' or tok_type == 'VOID_TEST_MAIN_INT_CHAR' or\
               tok_type == 'VOID_CPP_MAIN_VOID' or tok_type == 'VOID_CPP_MAIN_INT_CHAR' or\
               tok_type == 'INT_MAIN_INT_CHAR_CONST' or tok_type == 'INT_NEW_MAIN_VOID':

                if tok_type == 'INT_MAIN_VOID' or tok_type == 'VOID_MAIN_VOID':
                    main_call = "main()"
                elif tok_type == 'INT_MAIN_INT_CHAR' or tok_type == 'VOID_MAIN_INT_CHAR' or tok_type == 'INT_MAIN_INT_CHAR_CONST':
                    main_call = "main(0, NULL)"
                elif tok_type == 'INT_TEST_MAIN_VOID' or tok_type == 'VOID_TEST_MAIN_VOID':
                    main_call = "test_main()"
                elif tok_type == 'INT_TEST_MAIN_INT_CHAR' or tok_type == 'VOID_TEST_MAIN_INT_CHAR':
                    main_call = "test_main(0, NULL)"
                elif tok_type == 'INT_CPP_MAIN_VOID' or tok_type == 'VOID_CPP_MAIN_VOID':
                    main_call = "cpp_main()"
                elif tok_type == 'INT_NEW_MAIN_VOID':
                    main_call = "main()"
                else: #tok_type == 'INT_CPP_MAIN_INT_CHAR' or tok_type == 'VOID_CPP_MAIN_INT_CHAR':
                    main_call = "cpp_main(0, NULL)"

                if tok_type == 'INT_NEW_MAIN_VOID':
                    main_define = "int main();"
                else:
                    main_define = tok.value + ";"

                main_pos = tok_pos
                have_main = True

            if tok_type == 'INIT_UNIT_TEST_SUITE':
                unit_test_suite_pos = tok_pos
                have_unit_test_suite = True

            #if (tok_type <> 'SINGLE_LINE_COMMENT') and (tok_type <> 'MULTI_LINE_COMMENT') :
            #print tok.type, tok.value, tok.lexpos

#        print "last include pos = %s, len = %i", last_include_pos, include_len)

        if have_main or have_unit_test_suite:

            insert_at = last_include_pos + include_len

            output_file = input_file
            namespace_name = os.path.splitext(os.path.basename(input_file))[0]
            comment = "/*<boost.py:auto_insert>*/"

            os.rename(input_file, input_file + ".bak")

            file = open(output_file, "w")


            insert_text = """
#include "IwTest.h"

#undef BOOST_STATIC_ASSERT
#define BOOST_STATIC_ASSERT

#undef BOOST_TEST
#define BOOST_TEST(COND) IW_TEST_ASSERT_ACTION(COND, 1 == 1, (""))

#undef BOOST_CHECK
#define BOOST_CHECK(COND) BOOST_TEST(COND)

#undef BOOST_ERROR
#define BOOST_ERROR(MSG) IwTestError(MSG)

#undef BOOST_TEST_EQ
#define BOOST_TEST_EQ(VAL1, VAL2) IW_TEST_ASSERT_EQ(VAL1, VAL2)

#undef BOOST_TEST_NE
#define BOOST_TEST_NE(VAL1, VAL2) IW_TEST_ASSERT_NOTEQ(VAL1, VAL2)

#undef BOOST_CHECK_EQUAL
#define BOOST_CHECK_EQUAL(VAL1, VAL2) IW_TEST_ASSERT_COMPARATION_IMPL(VAL1, == , VAL2, 1==1)

#undef BOOST_CHECK_THROW
#define BOOST_CHECK_THROW(S, E)                                                  \\
    try {                                                                        \\
        S;                                                                       \\
        BOOST_ERROR("Exception was expected but not thrown - " #S);              \\
    }                                                                            \\
    catch( E const& ex ) {                                                       \\
        ::boost::unit_test::ut_detail::ignore_unused_variable_warning( ex );     \\
    }

#undef BOOST_CHECK_CLOSE_FRACTION
#define BOOST_CHECK_CLOSE_FRACTION( L, R, T )             \\
    BOOST_CHECK(::boost::test_tools::check_is_close(L, R, \\
        ::boost::test_tools::fraction_tolerance(T)))

"""

            insert_text = insert_text + "\n" + comment + " namespace " + namespace_name + "_ns {" + comment + "\n"
            stream = stream[:insert_at] + insert_text + stream[insert_at:]

            stream += "\n" + comment + "}" + comment
            stream += "\n\n"
            
            append_text = ""
            if have_main:
               append_text = get_test_case_text_for_main(namespace_name, main_call)
            
            if have_unit_test_suite:
               append_text = get_test_case_text_for_boost_suite(namespace_name)

            stream += append_text

            file.write(stream)
            file.flush()
            file.close()
            print "Input file adjusted: " + input_file

        else:
            print "no main function in " + input_file


def get_test_case_text_for_main(namespace_name, main_call):
    text = "IW_TEST(" + namespace_name + ")\n"
    text += "{\n"
    text += "    " + namespace_name + "_ns::" + main_call + ";\n"
    text += "    return S3E_TRUE;\n"
    text += "}\n"
    return text


def get_test_case_text_for_boost_suite(namespace_name):
    text = """
class iw_test_tree_visitor : public boost::unit_test::test_tree_visitor 
{
public:
    virtual void visit( boost::unit_test::test_case const& tc) 
    { 
        try 
        {

            tc.test_func()(); 
        }
        catch (...)
        {
            BOOST_ERROR("Unhandled exception was thrown");
        }
    }
};

"""
    text += "IW_TEST(" + namespace_name + ")\n"
    text += "{\n"
    text += "    iw_test_tree_visitor v;\n"
    text += "    boost::unit_test::test_suite* suite = " + namespace_name + "_ns::init_unit_test_suite(0, NULL);\n"
    text += "    const boost::unit_test::test_suite& s = *suite;\n"
    text += "    boost::unit_test::traverse_test_tree(s, v);\n"
    text += "    boost::unit_test::framework::clear();\n"
    text += "    return S3E_TRUE;\n"
    text += "}\n"
    return text

