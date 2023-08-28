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

  py::class_<luban::Features, std::shared_ptr<luban::Features>>(m, "Features")
      .def(py::init<>())
      .def(py::init<const std::string &>())
      .def(py::init<std::vector<std::string>>())
      .def(py::init<std::initializer_list<std::string>>())
      .def("__repr__", [](luban::Features &fea) -> std::string {
        return fea.stringnify();
      });

  py::class_<luban::FeaturesList, std::shared_ptr<luban::FeaturesList>>(
      m, "FeaturesList")
      .def(py::init<>())
      .def("append", &luban::FeaturesList::push)
      .def("__len__", [](luban::FeaturesList &list) { return list.size(); })
      .def("__getitem__", &luban::FeaturesList::operator[])
      .def("__setitem__", [](luban::FeaturesList &list, size_t i,
                             luban::SharedFeaturesPtr v) { list.set(i, v); })
      .def("__repr__", [](luban::FeaturesList &list) -> std::string {
        return list.stringnify();
      });

  py::class_<luban::Matrices, std::shared_ptr<luban::Matrices>>(m, "Matrices")
      .def("__len__", [](luban::Matrices &m) { return m.m_matrices.size(); })
      .def("__getitem__", &luban::Matrices::operator[])
      .def("__repr__", [](const luban::Matrices &m) -> std::string {
        int size = m.m_matrices.size();
        if (size == 0) {
          return "<class pyluban.Matrices: null>";
        }

        int64_t rows = m.m_matrices[0]->m_rows;
        int64_t cols = m.m_matrices[0]->m_cols;
        std::string type = "int64";
        if (m.m_matrices[0]->m_type == luban::DataType::kFloat32) {
          type = "float32";
        }
        std::string ret = "<class pyluban.Matrices: shape:(";
        ret.append(std::to_string(size));
        ret.append(",");
        ret.append(std::to_string(rows));
        ret.append(",");
        ret.append(std::to_string(cols));
        ret.append("), dtype: ");
        ret.append(type);
        ret.append(">");
        return ret;
      });

  py::class_<luban::Rows, std::shared_ptr<luban::Rows>>(m, "Rows")
      .def("__len__", [](luban::Rows &r) { return r.m_rows.size(); })
      .def("__getitem__", &luban::Rows::operator[])
      .def("__repr__", [](const luban::Rows &r) -> std::string {
        int size = r.m_rows.size();
        if (size == 0) {
          return "<class pyluban.Rows: null>";
        }

        int64_t cols = r.m_rows[0]->m_cols;
        std::string type = "int64";
        if (r.m_rows[0]->m_type == luban::DataType::kFloat32) {
          type = "float32";
        }
        std::string ret = "<class pyluban.Rows: shape:(";
        ret.append(std::to_string(size));
        ret.append(",");
        ret.append(std::to_string(cols));
        ret.append("), dtype: ");
        ret.append(type);
        ret.append(">");
        return ret;
      });

  py::class_<luban::Toolkit>(m, "Toolkit")
      .def(py::init<const std::string &>())
      .def("process", py::overload_cast<luban::SharedFeaturesPtr>(
                          &luban::Toolkit::process))
      .def("process", py::overload_cast<luban::SharedFeaturesListPtr>(
                          &luban::Toolkit::process))
      .def("__repr__", [](const luban::Toolkit &t) -> std::string {
        return "<pyluban.Toolkit>";
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