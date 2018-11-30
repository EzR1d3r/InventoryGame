#pragma once
#include <cmath>
#include <Python.h>
#include "ig_item.h"

class IG_PyCaller
{
public:
	IG_PyCaller();
	~IG_PyCaller();
	void setPyModule (const char * name);
	int callFunc(const char *name, IG_Item * Item1, IG_Item * Item2);
private:
	PyObject* __Py_module = nullptr;
};
