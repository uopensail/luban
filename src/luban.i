%module pyluban
%include "stdint.i"
%include "typemaps.i"
%include "std_vector.i"
%include "std_string.i"


%{
#include "pyluban.h"
%}

%typemap(in) (char* IN_ARRAY1, int DIM1){
    $1 = PyString_AsString($input);
    $2 = PyString_Size($input);
}

%apply (char* IN_ARRAY1, int DIM1) {(char* features, int len)}

namespace std
{
%template(UInt64Vector) std::vector<unsigned long long>;
}
%include "pyluban.h"