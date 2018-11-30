#include "ig_pyextension.h"
#include <QDebug>


template<> void IG_PyCaller::init_py_class<IG_Item>()
{
	static bool already_called = false;
	if (already_called) return;
	boost::python::class_<IG_Item> ("IG_Item")
			.def("setState", &IG_Item::setState)
			.def("getState", &IG_Item::getState)
			.def("getInteractType", &IG_Item::getInteractType);
	qDebug() << "called";
	already_called = true;
}

IG_PyCaller::IG_PyCaller()
{
	Py_Initialize();
	PyRun_SimpleString("import sys\n"
						"sys.path.append('f:/Projects/InventoryGame/pyscripts/')\n");

	init_py_class<IG_Item>();
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
