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
      .def("__len__", [](luban::Matrices &m) { return m.size(); })
      .def("__getitem__", &luban::Matrices::operator[])
      .def("__repr__", [](luban::Matrices &m) -> std::string {
        int size = m.size();
        if (size == 0) {
          return "<pyluban.Matrices: null>";
        }

        std::string ret = "<pyluban.Matrices: [";
        for (int i = 0; i < size; i++) {
          if (i > 0) {
            ret.append(", ");
          }
          ret.append("<shape: (");
          ret.append(std::to_string(m.m_matrices[i]->m_rows));
          ret.append(",");
          ret.append(std::to_string(m.m_matrices[i]->m_cols));
          ret.append("), dtype: ");
          if (m.m_matrices[i]->m_type == luban::DataType::kFloat32) {
            ret.append("float32");
          } else {
            ret.append("int64");
          }
          ret.append(">");
        }
        ret.append("]>");
        return ret;
      });

  py::class_<luban::Rows, std::shared_ptr<luban::Rows>>(m, "Rows")
      .def("__len__", [](luban::Rows &r) { return r.size(); })
      .def("__getitem__", &luban::Rows::operator[])
      .def("__repr__", [](luban::Rows &r) -> std::string {
        int64_t size = r.size();
        if (size == 0) {
          return "<pyluban.Rows: null>";
        }

        std::string ret = "<pyluban.Rows: [";
        for (int64_t i = 0; i < size; i++) {
          if (i > 0) {
            ret.append(", ");
          }
          ret.append("<shape: (");
          ret.append(std::to_string(r.m_rows[i]->m_cols));
          ret.append(",), dtype: ");
          if (r.m_rows[i]->m_type == luban::DataType::kFloat32) {
            ret.append("float32");
          } else {
            ret.append("int64");
          }
          ret.append(">");
        }
        ret.append("]>");
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
          format = py::format_descriptor<float>::format();
        }
        return py::buffer_info(r.m_data, r.m_size, format, 1, {r.m_cols},
                               {r.m_size});
      })
      .def("__repr__", [](const luban::Row &r) -> std::string {
        std::string format = py::format_descriptor<int64_t>::format();
        if (r.m_type == luban::DataType::kFloat32) {
          format = py::format_descriptor<float>::format();
        }
        return "<pyluban.luban.Row: shape: (" + std::to_string(r.m_cols) +
               ",), dtype:" + format + ">";
      });

  py::class_<luban::Matrix, std::shared_ptr<luban::Matrix>>(
      m, "Matrix", py::buffer_protocol())
      .def_buffer([](luban::Matrix &m) -> py::buffer_info {
        std::string format = py::format_descriptor<int64_t>::format();
        if (m.m_type == luban::DataType::kFloat32) {
          format = py::format_descriptor<float>::format();
        }
        return py::buffer_info(m.m_data, m.m_size, format, 2,
                               {m.m_rows, m.m_cols},
                               {m.m_size * m.m_cols, m.m_size});
      })
      .def("__repr__", [](const luban::Matrix &m) -> std::string {
        std::string format = py::format_descriptor<int64_t>::format();
        if (m.m_type == luban::DataType::kFloat32) {
          format = py::format_descriptor<float>::format();
        }
        return "<pyluban.luban.Matrix: shape: (" + std::to_string(m.m_rows) +
               "," + std::to_string(m.m_cols) + "), dtype:" + format + ">";
      });
}