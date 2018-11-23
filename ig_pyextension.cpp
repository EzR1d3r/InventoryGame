#include "ig_pyextension.h"
#include <QDebug>

IG_PyCaller::IG_PyCaller()
{
	Py_Initialize();
	PyRun_SimpleString("import sys\n"
						"sys.path.append('f:/Projects/InventoryGame/pyscripts/')\n");
}

IG_PyCaller::~IG_PyCaller()
{
	__Py_module = nullptr;
	Py_XDECREF(__Py_module);
	Py_FinalizeEx();
}

void IG_PyCaller::setPyModule(const char * name)
{
	PyObject* pName = PyUnicode_DecodeFSDefault(name);
	__Py_module = PyImport_Import(pName);
//	Py_DECREF(pName);
}

int IG_PyCaller::callFunc(const char *name, int arg_1, int arg_2)
{
	PyObject* pFunc   = nullptr;
	PyObject *pArgs   = nullptr;
	PyObject *pResult = nullptr;
	if (__Py_module == nullptr) return 0;

	pFunc = PyObject_GetAttrString(__Py_module, name);
	pArgs = PyTuple_New(2);
	PyTuple_SetItem (pArgs, 0, PyLong_FromLong(arg_1));
	PyTuple_SetItem (pArgs, 1, PyLong_FromLong(arg_2));

	pResult = PyObject_CallObject(pFunc, pArgs);

	int res = PyLong_AsLong(pResult);

	Py_XDECREF(pFunc);
	Py_XDECREF(pArgs);
	Py_XDECREF(pResult);
	return res;
}
