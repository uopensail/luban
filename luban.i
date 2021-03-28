%module pyluban
%include "typemaps.i"
%include "std_vector.i"
%include "std_string.i"
%template(UInt64Vector) std::vector<u_int_64>;

%{
#include "pyluban.h"
%}

%include "pyluban.h"