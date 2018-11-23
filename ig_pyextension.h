#pragma once
#include <cmath>
#include <Python.h>


class IG_PyCaller
{
public:
	IG_PyCaller();
	~IG_PyCaller();
	void setPyModule (const char * name);
	int callFunc(const char * name, int arg_1, int arg_2);
private:
	PyObject* __Py_module = nullptr;
};
