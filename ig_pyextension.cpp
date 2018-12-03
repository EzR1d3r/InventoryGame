#include "ig_pyextension.h"
#include "ig_utils.h"


template<> void IG_PyCaller::init_py_class<IG_Item>()
{
	static bool already_called = false;
	if (already_called) return;
	boost::python::class_<IG_Item> ("IG_Item")
			.def("setState", &IG_Item::setState)
			.def("getState", &IG_Item::getState)
			.def("getInteractType", &IG_Item::getInteractType)
			.def("getIndex", &IG_Item::getIndex);
	already_called = true;
}

template<> void IG_PyCaller::PyObj_As<void>(PyObject * pObj, bool return_default)
{
	Q_UNUSED(pObj);
	Q_UNUSED(return_default);
}
template<> bool IG_PyCaller::PyObj_As<bool>(PyObject * pObj, bool return_default)
{
	if (return_default)
		return false;
	else
		return pObj == Py_True ? true : false;
}

IG_PyCaller::IG_PyCaller()
{
	Py_Initialize();
	QString py_ext_path( IG_Utils::absPathFromLocal(PY_SCRIPTS_PATH) );
	QString py_exec_string = QString("import sys\n"
			"sys.path.append('%1')\n").arg(py_ext_path);

	PyRun_SimpleString( py_exec_string.toStdString().c_str() );

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
