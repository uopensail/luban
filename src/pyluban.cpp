#include <pybind11/numpy.h>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include "toolkit.h"

namespace py = pybind11;

PYBIND11_MODULE(pyluban, m) {
  py::class_<luban::FunctionConfigure>(m, "Function")
      .def(py::init<const std::string &>())
      .def("__repr__", [](luban::FunctionConfigure &o) -> std::string {
        return o.stringnify();
      });

  py::class_<luban::Operator>(m, "Operator")
      .def(py::init<>())
      .def("call", &luban::Operator::call)
      .def("__repr__", [](luban::Operator &o) -> std::string {
        return "<pyluban.Operator>";
      });

  py::class_<luban::Features>(m, "Features")
      .def(py::init<>())
      .def(py::init<const std::string &>())
      .def(py::init<std::vector<std::string>>())
      .def(py::init<std::initializer_list<std::string>>())
      .def("__repr__", [](luban::Features &fea) -> std::string {
        return fea.stringnify();
      });

  py::class_<luban::Placement>(m, "Placement")
      .def("matrices", &luban::Placement::matrices)
      .def("rows", &luban::Placement::rows)
      .def("__repr__", [](const luban::Placement &p) -> std::string {
        return "<pyluban.Placement>";
      });

  py::class_<luban::Toolkit>(m, "Toolkit")
      .def(py::init<const std::string &>())
      .def("process",
           py::overload_cast<luban::Features &, luban::Matrices &, int64_t>(
               &luban::Toolkit::process))
      .def("process", py::overload_cast<luban::Features &, luban::Rows &>(
                          &luban::Toolkit::process))
      .def("__repr__", [](const luban::Toolkit &t) -> std::string {
        return "<pyluban.Toolkit";
      });

  py::class_<luban::Row, std::shared_ptr<luban::Row>>(m, "Row",
                                                      py::buffer_protocol())
      .def_buffer([](luban::Row &r) -> py::buffer_info {
        std::string format = py::format_descriptor<int64_t>::format();
        if (r.m_type == luban::DataType::kFloat32) {
          format = py::format_descriptor<int64_t>::format();
        }
        return py::buffer_info(r.m_data, r.m_size, format, 1, {r.m_cols},
                               {r.m_size});
      });

  py::class_<luban::Matrix, std::shared_ptr<luban::Matrix>>(
      m, "Matrix", py::buffer_protocol())
      .def_buffer([](luban::Matrix &m) -> py::buffer_info {
        std::string format = py::format_descriptor<int64_t>::format();
        if (m.m_type == luban::DataType::kFloat32) {
          format = py::format_descriptor<int64_t>::format();
        }
        return py::buffer_info(m.m_data, m.m_size, format, 2,
                               {m.m_rows, m.m_cols},
                               {m.m_size * m.m_cols, m.m_size});
      });
}