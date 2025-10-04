#pragma once
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/functional.h>
#include <pybind11/chrono.h>
#include <EastWind.h>
#include <EastWind_Math.h>
#include <EastWind_Graphics.h>
#include "def.h"
using namespace EastWind;
namespace py = pybind11;

namespace EastWind {
#define MAX_SIZE 10
class PyVec {
public:
    struct VecBase {
        virtual ~VecBase() = default;
        virtual std::size_t size() const = 0;
        virtual py::object getitem(std::size_t index) const = 0;
        virtual void setitem(std::size_t index, py::handle value) = 0;
        virtual py::list to_list() const = 0;
        virtual std::string repr() const = 0;

        virtual std::shared_ptr<VecBase> add(const VecBase* other) const = 0;
        virtual std::shared_ptr<VecBase> sub(const VecBase* other) const = 0;
        virtual std::shared_ptr<VecBase> mul(py::handle scalar) const = 0;
        virtual std::shared_ptr<VecBase> div(py::handle scalar) const = 0;
        virtual py::object dot(const VecBase* other) const = 0;
        virtual std::shared_ptr<VecBase> normalized() const = 0;
        virtual std::shared_ptr<VecBase> cross(const VecBase* other) const = 0;
        virtual py::object norm(py::handle level) const = 0;

        virtual bool equal(const VecBase* other) const = 0;
    };

    template<typename T, std::size_t n>
    struct VecImpl : public VecBase {
        Vec<T, n> vec;
        VecImpl() = default;
        VecImpl(const std::vector<T>& data) {
            vec = Vec<T,n>(data);    
        }
        VecImpl(const Vec<T,n>& other) : vec(other) {}
        VecImpl(Vec<T,n>&& other) : vec(std::move(other)) {}

        virtual std::size_t size() const override { return n; }
        virtual py::object getitem(std::size_t index) const override { 
            if (index >= n) throw py::index_error();
            return py::cast(vec(index));
        };
        virtual void setitem(std::size_t index, py::handle value) override {
            if (index >= n) throw py::index_error();
            vec(index) = value.cast<T>();
        }
        virtual py::list to_list() const override {
            py::list result;
            for (std::size_t i = 0; i < n; ++i) {
                result.append(vec(i));
            }
            return result;
        }
        virtual std::string repr() const override {
            std::string repr = "Vec([";
            for (std::size_t i = 0; i < n; ++i) {
                repr += std::to_string(vec(i));
                if (i < n - 1) repr += ", ";
            }
            repr += "])";
            return repr;
        }

        virtual std::shared_ptr<VecBase> add(const VecBase* other) const override {
            const VecImpl<T,n>* o = dynamic_cast<const VecImpl<T,n>*>(other);
            if (!o) throw std::runtime_error("Dimension or type mismatch in vector addition");
            return std::make_shared<VecImpl<T,n>>(vec + o->vec);
        }
        virtual std::shared_ptr<VecBase> sub(const VecBase* other) const override {
            const VecImpl<T,n>* o = dynamic_cast<const VecImpl<T,n>*>(other);
            if (!o) throw std::runtime_error("Dimension or type mismatch in vector subtraction");
            return std::make_shared<VecImpl<T,n>>(vec - o->vec);
        }
        virtual std::shared_ptr<VecBase> mul(py::handle scalar) const override {
            T val = scalar.cast<T>();
            return std::make_shared<VecImpl<T,n>>(vec * val);
        }
        virtual std::shared_ptr<VecBase> div(py::handle scalar) const override {
            T val = scalar.cast<T>();
            return std::make_shared<VecImpl<T,n>>(vec / val);
        }
        virtual py::object dot(const VecBase* other) const override {
            const VecImpl<T,n>* o = dynamic_cast<const VecImpl<T,n>*>(other);
            if (!o) throw std::runtime_error("Dimension or type mismatch in vector dot product");
            return py::cast(vec.dot(o->vec));
        }
        virtual std::shared_ptr<VecBase> normalized() const override {
            Vec<T,n> normed = vec;
            normed.normalize();
            return std::make_shared<VecImpl<T,n>>(normed);
        }
        virtual std::shared_ptr<VecBase> cross(const VecBase* other) const override {
            const VecImpl<T,n>* o = dynamic_cast<const VecImpl<T,n>*>(other);
            if (!o) throw std::runtime_error("Dimension or type mismatch in vector cross product");
            return std::make_shared<VecImpl<T,n>>(vec.cross(o->vec));
        }
        virtual py::object norm(py::handle level) const override {
            char lvl = level.cast<char>();
            return py::cast(vec.norm(lvl));
        }
        virtual bool equal(const VecBase* other) const override {
            const VecImpl<T,n>* o = dynamic_cast<const VecImpl<T,n>*>(other);
            if (!o) return false;
            if (n != o->size()) return false;
            return vec == o->vec;
        }
    };
private:
    std::shared_ptr<VecBase> _impl = nullptr;
    std::size_t _dim = 0;
    std::string _dtype = "";

    template<typename T>
    using CreatorFunc = std::function<std::shared_ptr<VecBase>(const std::vector<T>&)>;
    
    template<typename T, std::size_t... Sizes>
    static auto make_creator_table(std::index_sequence<Sizes...>) {
        return std::array<CreatorFunc<T>, sizeof...(Sizes)>{
            [](const std::vector<T>& data)->std::shared_ptr<VecBase> { 
                return std::make_shared<VecImpl<T, Sizes+1>>(data); 
            }...
        };
    }
    
    template<typename T>
    static const auto& get_creator_table() {
        static auto creators = make_creator_table<T>(std::make_index_sequence<MAX_SIZE>{});
        return creators;
    }

    template<typename T>
    static std::shared_ptr<VecBase> create_vec(const std::vector<T>& data) {
        if (data.size()-1 >= get_creator_table<T>().size()) {
            throw std::runtime_error("Unsupported vector size: " + std::to_string(data.size()));
        }
        return get_creator_table<T>()[data.size()-1](data);
    }
public:
    PyVec(std::shared_ptr<VecBase> impl) : _impl(impl) {}
    PyVec(const py::list& lst, const std::string& dtype = "float") {
        std::size_t size = lst.size();
        
        if (dtype == "float") {
            std::vector<float> data;
            for (auto item : lst) data.push_back(item.cast<float>());
            _impl = create_vec<float>(data);
        } 
        else if (dtype == "double") {
            std::vector<double> data;
            for (auto item : lst) data.push_back(item.cast<double>());
            _impl = create_vec<double>(data);
        }
        else if (dtype == "int") {
            std::vector<int> data;
            for (auto item : lst) data.push_back(item.cast<int>());
            _impl = create_vec<int>(data);
        }
        else {
            throw std::runtime_error("Unsupported dtype: " + dtype);
        }
    }
    

    template<typename T, std::size_t n>
    operator Vec<T, n>() const {
        if (auto derived = dynamic_cast<const VecImpl<T, n>*>(_impl.get())) {
            return derived->vec;
        }
        throw std::runtime_error("Cannot convert PyVec to Vec<T, n>");
    }

    py::object getitem(std::size_t index) const { return _impl->getitem(index); }
    void setitem(std::size_t index, py::handle value) { _impl->setitem(index, value); }
    std::size_t size() const { return _impl->size(); }
    py::list to_list() const { return _impl->to_list(); }
    std::string repr() const { return _impl->repr(); }

    PyVec add(const PyVec& other) const {
        return PyVec(_impl->add(other._impl.get()));
    }
    
    PyVec sub(const PyVec& other) const {
        return PyVec(_impl->sub(other._impl.get()));
    }
    
    PyVec mul(py::handle scalar) const {
        return PyVec(_impl->mul(scalar));
    }
    
    PyVec div(py::handle scalar) const {
        return PyVec(_impl->div(scalar));
    }
    
    py::object dot(const PyVec& other) const {
        return _impl->dot(other._impl.get());
    }

    PyVec normalized() const {
        return PyVec(_impl->normalized());
    }
    
    PyVec cross(const PyVec& other) const {
        return PyVec(_impl->cross(other._impl.get()));
    }

    py::object norm(py::handle level) const {
        return _impl->norm(level);
    }

    bool operator==(const PyVec& other) const {
        return _impl->equal(other._impl.get());
    }
    bool operator!=(const PyVec& other) const {
        return !_impl->equal(other._impl.get());
    }
};

class PyMat {
public:
    struct MatBase {
        virtual ~MatBase() = default;
        virtual std::size_t rows() const = 0;
        virtual std::size_t cols() const = 0;
        virtual py::object getitem(std::size_t i, std::size_t j) const = 0;
        virtual void setitem(std::size_t i, std::size_t j, py::handle value) = 0;
        virtual py::list to_list() const = 0;
        virtual std::string repr() const = 0;

        virtual std::shared_ptr<MatBase> add(const MatBase* other) const = 0;
        virtual std::shared_ptr<MatBase> sub(const MatBase* other) const = 0;
        virtual std::shared_ptr<MatBase> mul(py::handle scalar) const = 0;
        virtual std::shared_ptr<MatBase> div(py::handle scalar) const = 0;
        virtual std::shared_ptr<MatBase> matmul(const MatBase* other) const = 0;
        virtual py::object norm(py::handle level) const = 0;
        virtual std::shared_ptr<MatBase> transpose() const = 0;
        virtual std::shared_ptr<MatBase> inverse() const = 0;

        virtual bool equal(const MatBase* other) const = 0;
    };

    template<typename T, std::size_t m, std::size_t n>
    struct MatImpl : public MatBase {
        Mat<T, m, n> mat;
        MatImpl() = default;
        MatImpl(const std::vector<std::vector<T>>& data) {
            for (std::size_t i = 0; i < m; ++i)
                for (std::size_t j = 0; j < n; ++j)
                    mat(i, j) = data[i][j];
        }
        MatImpl(const Mat<T, m, n>& other) : mat(other) {}
        MatImpl(Mat<T, m, n>&& other) : mat(std::move(other)) {}

        virtual std::size_t rows() const override { return m; }
        virtual std::size_t cols() const override { return n; }
        virtual py::object getitem(std::size_t i, std::size_t j) const override {
            if (i >= m || j >= n) throw py::index_error();
            return py::cast(mat(i, j));
        }
        virtual void setitem(std::size_t i, std::size_t j, py::handle value) override {
            if (i >= m || j >= n) throw py::index_error();
            mat(i, j) = value.cast<T>();
        }
        virtual py::list to_list() const override {
            py::list result;
            for (std::size_t i = 0; i < m; ++i) {
                py::list row;
                for (std::size_t j = 0; j < n; ++j)
                    row.append(mat(i, j));
                result.append(row);
            }
            return result;
        }
        virtual std::string repr() const override {
            std::string s = "Mat([\n";
            for (std::size_t i = 0; i < m; ++i) {
                s += "[";
                for (std::size_t j = 0; j < n; ++j) {
                    s += std::to_string(mat(i, j));
                    if (j < n - 1) s += ", ";
                }
                s += "]\n";
                // if (i < m - 1) s += ", ";
            }
            s += "])";
            return s;
        }

        virtual std::shared_ptr<MatBase> add(const MatBase* other) const override {
            auto o = dynamic_cast<const MatImpl<T, m, n>*>(other);
            if (!o) throw std::runtime_error("Dimension or type mismatch in matrix addition");
            return std::make_shared<MatImpl<T, m, n>>(mat + o->mat);
        }
        virtual std::shared_ptr<MatBase> sub(const MatBase* other) const override {
            auto o = dynamic_cast<const MatImpl<T, m, n>*>(other);
            if (!o) throw std::runtime_error("Dimension or type mismatch in matrix subtraction");
            return std::make_shared<MatImpl<T, m, n>>(mat - o->mat);
        }
        virtual std::shared_ptr<MatBase> mul(py::handle scalar) const override {
            T val = scalar.cast<T>();
            Mat<T, m, n> result = mat;
            for (std::size_t i = 0; i < m; ++i)
                for (std::size_t j = 0; j < n; ++j)
                    result(i, j) *= val;
            return std::make_shared<MatImpl<T, m, n>>(result);
        }
        virtual std::shared_ptr<MatBase> div(py::handle scalar) const override {
            T val = scalar.cast<T>();
            Mat<T, m, n> result = mat;
            for (std::size_t i = 0; i < m; ++i)
                for (std::size_t j = 0; j < n; ++j)
                    result(i, j) /= val;
            return std::make_shared<MatImpl<T, m, n>>(result);
        }
        virtual std::shared_ptr<MatBase> matmul(const MatBase* other) const override {
            // Only support multiplication with compatible MatImpl
            // If n != k, throw
            // For simplicity, only support square for now
            auto o = dynamic_cast<const MatImpl<T, n, n>*>(other);
            if (!o) throw std::runtime_error("Dimension or type mismatch in matrix multiplication");
            return std::make_shared<MatImpl<T, m, n>>(mat * o->mat);
        }
        virtual py::object norm(py::handle level) const override {
            char lvl = level.cast<char>();
            return py::cast((double)(mat.norm(lvl)));
        }
        virtual std::shared_ptr<MatBase> transpose() const override {
            return std::make_shared<MatImpl<T, n, m>>(mat.Transpose());
        }
        virtual std::shared_ptr<MatBase> inverse() const override {
            if constexpr (m != n)
                throw std::runtime_error("Inverse only for square matrices");
            else
                return std::make_shared<MatImpl<T, m, n>>(mat.Inverse());
        }
        virtual bool equal(const MatBase* other) const override {
            auto o = dynamic_cast<const MatImpl<T, m, n>*>(other);
            if (!o) return false;
            return mat == o->mat;
        }
    };

private:
    std::shared_ptr<MatBase> _impl = nullptr;
    std::size_t _rows = 0, _cols = 0;
    std::string _dtype = "";

    template<typename T>
    using CreatorFunc = std::function<std::shared_ptr<MatBase>(const std::vector<std::vector<T>>&)>;

    template<typename T, std::size_t M, std::size_t N>
    static CreatorFunc<T> make_creator() {
        return [](const std::vector<std::vector<T>>& data)->std::shared_ptr<MatBase> { 
            return std::make_shared<MatImpl<T, M, N>>(data); 
        };
    }

    template<typename T, std::size_t M, std::size_t... Ns>
    static auto make_row(std::index_sequence<Ns...>) {
        return std::array<CreatorFunc<T>, sizeof...(Ns)>{
            make_creator<T, M, Ns + 1>()...
        };
    }

    template<typename T, std::size_t... Ms, std::size_t... Ns>
    static auto make_creator_table(std::index_sequence<Ms...>, std::index_sequence<Ns...>) {
        auto rows = std::array{
            make_row<T, Ms + 1>(std::make_index_sequence<sizeof...(Ns)>{})...
        };
        
        static std::array<CreatorFunc<T>, sizeof...(Ms) * sizeof...(Ns)> result;
        std::size_t index = 0;
        for (const auto& row : rows) {
            for (const auto& creator : row) {
                result[index++] = creator;
            }
        }
        return result;
    }

    template<typename T>
    static const auto& get_creator_table() {
        static auto creators = make_creator_table<T>(
            std::make_index_sequence<MAX_SIZE>{},
            std::make_index_sequence<MAX_SIZE>{}
        );
        return creators;
    }

    template<typename T>
    static std::shared_ptr<MatBase> create_mat(const std::vector<std::vector<T>>& data) {
        std::size_t m = data.size();
        std::size_t n = m > 0 ? data[0].size() : 0;
        if (m == 0 || n == 0 || m > MAX_SIZE || n > MAX_SIZE)
            throw std::runtime_error("Unsupported matrix size");
        return get_creator_table<T>()[(m - 1) * MAX_SIZE + (n - 1)](data);
    }

public:
    PyMat(std::shared_ptr<MatBase> impl) : _impl(impl) {}
    PyMat(const py::list& lst, const std::string& dtype = "float") {
        std::vector<std::vector<float>> data_f;
        std::vector<std::vector<double>> data_d;
        std::vector<std::vector<int>> data_i;
        std::size_t m = lst.size();
        std::size_t n = m > 0 ? py::len(lst[0]) : 0;
        if (dtype == "float") {
            for (auto row : lst) {
                std::vector<float> rowvec;
                for (auto item : py::cast<py::list>(row))
                    rowvec.push_back(item.cast<float>());
                data_f.push_back(rowvec);
            }
            _impl = create_mat<float>(data_f);
        } else if (dtype == "double") {
            for (auto row : lst) {
                std::vector<double> rowvec;
                for (auto item : py::cast<py::list>(row))
                    rowvec.push_back(item.cast<double>());
                data_d.push_back(rowvec);
            }
            _impl = create_mat<double>(data_d);
        } else if (dtype == "int") {
            for (auto row : lst) {
                std::vector<int> rowvec;
                for (auto item : py::cast<py::list>(row))
                    rowvec.push_back(item.cast<int>());
                data_i.push_back(rowvec);
            }
            _impl = create_mat<int>(data_i);
        } else {
            throw std::runtime_error("Unsupported dtype: " + dtype);
        }
    }

    template<typename T, std::size_t m, std::size_t n>
    operator Mat<T, m, n>() const {
        if (auto derived = dynamic_cast<const MatImpl<T, m, n>*>(_impl.get())) {
            return derived->mat;
        }
        throw std::runtime_error("Cannot convert PyMat to Mat<T, m, n>");
    }

    py::object getitem(std::size_t i, std::size_t j) const { return _impl->getitem(i, j); }
    void setitem(std::size_t i, std::size_t j, py::handle value) { _impl->setitem(i, j, value); }
    std::size_t rows() const { return _impl->rows(); }
    std::size_t cols() const { return _impl->cols(); }
    py::list to_list() const { return _impl->to_list(); }
    std::string repr() const { return _impl->repr(); }

    PyMat add(const PyMat& other) const { return PyMat(_impl->add(other._impl.get())); }
    PyMat sub(const PyMat& other) const { return PyMat(_impl->sub(other._impl.get())); }
    PyMat mul(py::handle scalar) const { return PyMat(_impl->mul(scalar)); }
    PyMat div(py::handle scalar) const { return PyMat(_impl->div(scalar)); }
    PyMat matmul(const PyMat& other) const { return PyMat(_impl->matmul(other._impl.get())); }
    py::object norm(py::handle level) const { return _impl->norm(level); }
    PyMat transpose() const { return PyMat(_impl->transpose()); }
    PyMat inverse() const { return PyMat(_impl->inverse()); }

    bool operator==(const PyMat& other) const { return _impl->equal(other._impl.get()); }
    bool operator!=(const PyMat& other) const { return !_impl->equal(other._impl.get()); }
};

#undef MAX_SIZE
}