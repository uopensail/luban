%module luban
%include "stdint.i"
%include "typemaps.i"
%include "std_vector.i"
%include "std_string.i"
%include "cpointer.i"

%{
#define SWIG_FILE_WITH_INIT
#include "pyluban.h"
%}

%typemap(in) (char* IN_ARRAY1, int DIM1){
    $1 = PyString_AsString($input);
    $2 = PyString_Size($input);
}

%apply (char* IN_ARRAY1, int DIM1) {(char* features, int len)}

%include "pyluban.h"
%template(Array) std::vector<int64_t>;