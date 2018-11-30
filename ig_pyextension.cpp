#include "ig_pyextension.h"
//#include <boost/core/noncopyable.hpp>
//#include <boost/python/reference_existing_object.hpp>
#include <boost/python/ptr.hpp>
#include <boost/python.hpp>
//#include <boost/shared_ptr.hpp>
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

int IG_PyCaller::callFunc(const char *name, IG_Item *Item1, IG_Item *Item2)
{
	qDebug() << "==========";
	boost::python::class_<IG_Item> ("IG_Item")
			.def("setState", &IG_Item::setState)
			.def("getState", &IG_Item::getState);
//	boost::python::register_ptr_to_python<IG_Item*>();
	boost::python::object * pObj1 = new boost::python::object( boost::python::ptr (Item1) );
	boost::python::object * pObj2 = new boost::python::object( boost::python::ptr (Item2) );

	PyObject* pFunc   = nullptr;
	PyObject *pArgs   = nullptr;
	PyObject *pResult = nullptr;
	if (__Py_module == nullptr) return 0;

	pFunc = PyObject_GetAttrString(__Py_module, name);
	pArgs = PyTuple_New(2);
	PyTuple_SetItem (pArgs, 0, pObj1->ptr());
	PyTuple_SetItem (pArgs, 1, pObj2->ptr());

	qDebug() << "PyObj: " << pObj1->ptr() << pObj2->ptr();
	qDebug() << "1: " << Item1 <<Item1->getState() << Item2 << Item2->getState();
	pResult = PyObject_CallObject(pFunc, pArgs);
//	int res = PyLong_AsLong(pResult);
//	PyObject_CallObject(pFunc, pArgs);
	qDebug() << "2: " << Item1 <<Item1->getState() << Item2 << Item2->getState() << pResult;

	Py_XDECREF(pFunc);
	Py_XDECREF(pArgs);
	Py_XDECREF(pResult);
	return 1;
}
