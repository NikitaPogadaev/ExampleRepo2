#include <Python.h>
#include <iostream>

int main() {
    std::string name;
    std::cin >> name;
    Py_Initialize();

    PyRun_SimpleString("import sys; sys.path.append('.')");

    PyObject* moduleName = PyUnicode_FromString("api");
    PyObject* module = PyImport_Import(moduleName);
    Py_DECREF(moduleName);

    if (!module) {
        PyErr_Print();
        return 1;
    }

    PyObject* greetFunc = PyObject_GetAttrString(module, "greet");

    if (greetFunc && PyCallable_Check(greetFunc)) {
        PyObject* args = PyTuple_Pack(1, PyUnicode_FromString(name.c_str()));
        PyObject* result = PyObject_CallObject(greetFunc, args);

        if (result) {
            std::cout << PyUnicode_AsUTF8(result) << std::endl;
            Py_DECREF(result);
        } else {
            PyErr_Print();
        }

        Py_DECREF(args);
    }

    Py_XDECREF(greetFunc);
    Py_DECREF(module);

    Py_Finalize();
    return 0;
}