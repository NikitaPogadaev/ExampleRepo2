#include <pybind11/embed.h>
#include <iostream>

namespace py = pybind11;

int main() {
    std::string name;
    std::cin >> name;
    py::scoped_interpreter guard{};

    py::module_ sys = py::module_::import("sys");
    sys.attr("path").attr("append")(".");

    py::module_ api = py::module_::import("api");
    py::object result = api.attr("greet")(name);

    std::cout << result.cast<std::string>() << std::endl;

    return 0;
}