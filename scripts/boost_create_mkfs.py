#!/usr/bin/python

import os                                
import boost_convert_tests
            
class Mkf:
    @property
    def path(self):
        return self.__path

    @path.setter
    def path(self, value):
        self.__path = value

    @property
    def name(self):
        return self.__name

    @name.setter
    def name(self, value):
        self.__name = value

    @property
    def full_name(self):
        return os.path.join(self.path, self.name)

    @property
    def source_files(self):
        return self.__source_files

    @property
    def subprojects(self):
        return self.__subprojects

    @property
    def defines(self):
        return self.__defines

    @property
    def options(self):
        return self.__options

    @property
    def includepaths(self):
        return self.__includepaths


    def __init__(self):
        self.__path = ""
        self.__name = ""
        self.__source_files = []
        self.__subprojects = []
        self.__module_paths = []
        self.__defines = []
        self.__options = []
        self.__includepaths = []


# Exclusions allow to tune conversion algorythm in case automatic
# conversion is not working 

def check_file_exclusions(file_name):
    exclusions = [\
        'Jamfile.v2', 
        'libs\\thread\\src\\win32',                            # we support threads on top of pthreads only
        'libs\\smart_ptr\\test\\weak_ptr_move_test.cpp',       # std::move is not available
        'libs\\smart_ptr\\test\\shared_ptr_move_test.cpp',     # std::move is not available
        'libs\\smart_ptr\\test\\intrusive_ptr_move_test.cpp',  # std::move is not available
        'libs\\smart_ptr\\test\\scoped_array_eq_fail.cpp',     # compilation fail test
        'libs\\smart_ptr\\test\\shared_ptr_compare_fail.cpp',  # compilation fail test
        'libs\\smart_ptr\\test\\shared_ptr_delete_fail.cpp',   # compilation fail test
        'libs\\smart_ptr\\test\\shared_ptr_assign_fail.cpp',   # compilation fail test
        'libs\\smart_ptr\\test\\auto_ptr_lv_fail.cpp',         # compilation fail test
        'libs\\smart_ptr\\test\\shared_ptr_pv_fail.cpp',       # compilation fail test
        'libs\\smart_ptr\\test\\scoped_ptr_eq_fail.cpp',       # compilation fail test
        'libs\\smart_ptr\\test\\collector_test.cpp',           # smart_ptr collector is disabled with Marmalade
        'libs\\test\\src\\test_main.cpp',                      # to avoid multiple definitions of main
        'libs\\conversion\\test\\implicit_cast_fail.cpp',      # compilation fail test
        'libs\\regex\\test\\c_compiler_checks\\wide_posix_api_check.c', # compiler test
        'libs\\regex\\test\\c_compiler_checks\\posix_api_check.c',      # compiler test
        'libs\\regex\\test\\test_consolidated.cpp',            #
        'libs\\config\\test\\config_info.cpp',                 # support utility to print config parameters
        'libs\\regex\\test\\collate_info\\collate_info.cpp',   # 
        'libs\\regex\\test\\concepts\\icu_concept_check.cpp',  # 
        'libs\\regex\\test\\config_info\\regex_config_info.cpp',  # support utility to print config parameters
        'libs\\system\\test\\header_only_test.cpp',

    ]

    for ex in exclusions:
        if file_name.find(ex) >= 0:
            return True

    return False
 

def dump_mkf_command_list(file, command_name, list):
    if len(list) == 0:
        return

    file.writelines(command_name + "\n")
    file.writelines("{\n")
    
    for i in list:
        file.writelines("    " + i)
        file.writelines("\n")

    file.writelines("}\n\n")


def create_mkf_file(path, mkf):
    mkf_full_name = os.path.join(path, mkf.name)
    print mkf_full_name

    file = open(mkf_full_name, "w")

    dump_mkf_command_list(file, "defines", mkf.defines)
    dump_mkf_command_list(file, "includepaths", mkf.includepaths)
    dump_mkf_command_list(file, "options", mkf.options)
    dump_mkf_command_list(file, "files", mkf.source_files)
    dump_mkf_command_list(file, "subprojects", mkf.subprojects)

    file.flush()
    file.close()


def process_single_source_dir(mkf, path, mkf_relative_path, convert_tests):
    enum_dir = os.listdir(path)

    for file_name in enum_dir:
        full_name = os.path.join(path, file_name)
        full_relative_name = os.path.join(mkf_relative_path, file_name)

        if not os.path.isdir(full_name):

            if check_file_exclusions(full_name):
                continue

            mkf.source_files.append(full_relative_name)
            
            if convert_tests:
                test_converter = boost_lex.BoostTestConverter()
                test_converter.process(full_name)

        else:
            process_single_source_dir(mkf, full_name, full_relative_name, convert_tests)



def process_headers_dirs(mkb, path):
    path = os.path.join(path, "boost")
    enum_dir = os.listdir(path)

    top_level_headers_mkf = Mkf()
    top_level_headers_mkf.name = "boost_top_level_headers.mkf"
    top_level_headers_mkf.path = "boost"


    for file_name in enum_dir:
        full_name = os.path.join(path, file_name)

        if not os.path.isdir(full_name):
            file_relative_name = os.path.join(file_name)
            top_level_headers_mkf.source_files.append(file_relative_name)

        else:
            headers_mkf = Mkf()
            headers_mkf.name = file_name + ".mkf"
            headers_mkf.path = os.path.join("boost", file_name)
            process_single_source_dir(headers_mkf, full_name, "", False)
            create_mkf_file(full_name, headers_mkf)
            mkb.subprojects.append(headers_mkf.full_name)

    mkb.subprojects.append(top_level_headers_mkf.full_name)
    create_mkf_file(path, top_level_headers_mkf)



def process_lib_dirs(mkb, path, parent_lib_dir, source_dir, suffix):
    path = os.path.join(path, parent_lib_dir)
    enum_dir = os.listdir(path)

    for file_name in enum_dir:
        full_name = os.path.join(path, file_name)

        if os.path.isdir(full_name):
            full_name = os.path.join(full_name, source_dir)
            if os.path.exists(full_name):
                lib_mkf = Mkf()
                lib_mkf.name = file_name + suffix + ".mkf"
            
                #construct relative path of mkf within mkb
                lib_mkf.path = os.path.join(parent_lib_dir, file_name, source_dir)
                process_single_source_dir(lib_mkf, full_name, "", True)
                create_mkf_file(full_name, lib_mkf)
                mkb.subprojects.append(lib_mkf.full_name)



# entry point
def main():

    mkb = Mkf()
    mkb.path = "in"
    mkb.name = "boost.mkf"

    mkb.defines.append("BOOST_NO_SWPRINTF")

    mkb.defines.append("BOOST_HAS_THREADS")
    mkb.defines.append("BOOST_ALL_NO_LIB")
    mkb.defines.append("IGNORE_TICKET_2098")
    mkb.defines.append("_BIG_ENDIAN")

    mkb.includepaths.append("./")

    mkb.options.append("enable-exceptions")

    process_headers_dirs(mkb, "in")
    process_lib_dirs(mkb, "in", "libs", "src", "_lib")

    create_mkf_file("in", mkb)

    # create mkb for tests
    mkb = Mkf()
    mkb.path = "in"
    mkb.name = "boost_tests.mkb"

    mkb.defines.append("BOOST_TEST_NO_MAIN")
    mkb.defines.append("BOOST_HAS_THREADS")
    mkb.defines.append("BOOST_ALL_NO_LIB")
    mkb.defines.append("IGNORE_TICKET_2098")
    mkb.defines.append("_BIG_ENDIAN")
    mkb.defines.append("IW_CANARY_UI")
    mkb.defines.append("IW_BUILD_TESTS=1")

    mkb.includepaths.append("./")

    mkb.options.append("enable-exceptions")

    process_lib_dirs(mkb, "in", "libs", "test", "_test")

    mkb.subprojects.append("boost");
    mkb.subprojects.append("canary");

    create_mkf_file("in", mkb)

    
main()
