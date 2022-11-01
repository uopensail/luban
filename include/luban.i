%module pyluban
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

namespace std
{
  %template(UInt64Vector) std::vector<unsigned long long>;
  %template(FloatVector) std::vector<float>;
}

%typemap(out) Entity * {
  static bool isEntityTypeInit = false;
  static PyStructSequence_Field entity_fields[] = {
      {"gid", "group id of feature"},
      {"data", "uint64 list"},
      {NULL}};
  static PyStructSequence_Desc entity__desc = {
      "Entity",
      NULL,
      entity_fields,
      2};
  static PyTypeObject EntityType = {{{0, 0}, 0}, 0, 0, 0};
  if (!isEntityTypeInit)
  {
    PyStructSequence_InitType(&EntityType, &entity__desc);
    isEntityTypeInit = true;
  }
  $result = PyStructSequence_New(&EntityType);
  PyObject *gid = PyLong_FromSsize_t($1->gid);
  PyObject *data = PyList_New($1->size);
  for (size_t j = 0; j < $1->size; ++j)
  {
    PyList_SET_ITEM(data, j, PyLong_FromSsize_t($1->data[j]));
  }
  PyStructSequence_SetItem($result, 0, gid);
  PyStructSequence_SetItem($result, 1, data);
}
%include "pyluban.h"