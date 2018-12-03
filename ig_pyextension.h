#pragma once
#include <cmath>
#include <Python.h>
#include <boost/python/ptr.hpp>
#include <boost/python.hpp>
#include "ig_item.h"

class IG_PyCaller
{
public:
	IG_PyCaller();
	~IG_PyCaller();
	template <typename T>void init_py_class();
	void setPyModule (const char * name);
	template<typename T> T PyObj_As(PyObject* pObj, bool return_default = false);
	template <typename T, typename Res = void> Res callFunc(const char *name, T * pObj1, T * pObj2);
private:
	PyObject* __Py_module = nullptr;
};

template <typename T, typename Res> Res IG_PyCaller::callFunc(const char *name, T * pObj1, T * pObj2)
{
	boost::python::object * pPyObj1 = new boost::python::object( boost::python::ptr (pObj1) );
	boost::python::object * pPyObj2 = new boost::python::object( boost::python::ptr (pObj2) );

	PyObject* pFunc   = nullptr;
	PyObject *pArgs   = nullptr;
	PyObject *pResult = nullptr;

	//возврат значения "по умолчанию", если модуль не найден
	//не во всех случаях будет корректно, но пока устраивает
	if (__Py_module == nullptr) return PyObj_As<Res>(pResult, true);

	pFunc = PyObject_GetAttrString(__Py_module, name);
	pArgs = PyTuple_New(2);
	PyTuple_SetItem (pArgs, 0, pPyObj1->ptr());
	PyTuple_SetItem (pArgs, 1, pPyObj2->ptr());

	pResult = PyObject_CallObject(pFunc, pArgs);

	Py_XDECREF(pFunc);
	Py_XDECREF(pArgs);
	Py_XDECREF(pResult);

	return PyObj_As<Res>(pResult);
}

