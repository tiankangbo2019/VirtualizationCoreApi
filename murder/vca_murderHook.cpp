//
// Created by root on 2019/12/16.
//

#include "vca_murderHook.h"

vca_murderHook::vca_murderHook() {

    //init
    Py_Initialize();
    assert(Py_IsInitialized());
    const char *strcmd = "sys.path.append('../murder')";

    PyRun_SimpleString("import sys");
    PyRun_SimpleString("import os");
    PyRun_SimpleString(strcmd);
    //import模块
    p2p_Module = PyImport_ImportModule(murder.p2p);
    assert(p2p_Module);

}

vca_murderHook::~vca_murderHook() {
    Py_DECREF(p2p_Module);
}


bool vca_murderHook::startTracker() {
    PyObject *pFunc = nullptr;
    PyObject *pDict = nullptr;
    int code;
    const char *data;

    pFunc = PyObject_GetAttrString(p2p_Module, p2pmthod.startTracker);
    if (!pFunc || !PyCallable_Check(pFunc)) {
        Py_DECREF(pFunc);
        return false;
    }

    pDict = PyEval_CallObject(pFunc, nullptr);
    if (nullptr == pDict) {
        Py_DECREF(pDict);
        return false;
    }

    if (2 != PyDict_Size(pDict)) {
        return false;
    }

    PyObject *pcode = PyDict_GetItemString(pDict, "code");
    PyObject *pvalue = PyDict_GetItemString(pDict, "data");

    PyArg_Parse(pcode, "i", &code);
    PyArg_Parse(pvalue, "s", &data);

    if(code < 1){
        Py_DECREF(pFunc);
        Py_DECREF(pDict);

        return false;
    }

    Py_DECREF(pFunc);
    Py_DECREF(pDict);
    return true;
}

bool vca_murderHook::stopTracker() {
    PyObject *pFunc = nullptr;
    PyObject *pDict = nullptr;
    int code;
    const char *data;

    pFunc = PyObject_GetAttrString(p2p_Module, p2pmthod.stopTracker);
    if (!pFunc || !PyCallable_Check(pFunc)) {
        Py_DECREF(pFunc);
        return false;
    }

    pDict = PyEval_CallObject(pFunc, nullptr);
    if (nullptr == pDict) {
        Py_DECREF(pDict);
        return false;
    }

    if (2 != PyDict_Size(pDict)) {
        return false;
    }

    PyObject *pcode = PyDict_GetItemString(pDict, "code");
    PyObject *pvalue = PyDict_GetItemString(pDict, "data");

    PyArg_Parse(pcode, "i", &code);
    PyArg_Parse(pvalue, "s", &data);

    if(code < 1){
        Py_DECREF(pFunc);
        Py_DECREF(pDict);

        return false;
    }

    Py_DECREF(pFunc);
    Py_DECREF(pDict);
    return true;
}

bool vca_murderHook::produceSeed(const string &sourceFilePath,
                                 const string &sourceSeedPath,
                                 const string &localHost,
                                 const string &targetHost,
                                 const string &targetSeedPath) {

    PyObject *pFunc = nullptr;
    PyObject *pDict = nullptr;
    PyObject* args = PyTuple_New(5);

    int code;
    const char *data;

    PyTuple_SetItem(args, 0, Py_BuildValue("s", sourceFilePath.c_str()));
    PyTuple_SetItem(args, 1, Py_BuildValue("s", sourceSeedPath.c_str()));
    PyTuple_SetItem(args, 2, Py_BuildValue("s", localHost.c_str()));
    PyTuple_SetItem(args, 3, Py_BuildValue("s", targetHost.c_str()));
    PyTuple_SetItem(args, 4, Py_BuildValue("s", targetSeedPath.c_str()));

    pFunc = PyObject_GetAttrString(p2p_Module, p2pmthod.produceSeed);
    if (!pFunc || !PyCallable_Check(pFunc)) {
        Py_DECREF(pFunc);
        return false;
    }

    pDict = PyEval_CallObject(pFunc, args);
    if (nullptr == pDict) {
        Py_DECREF(pDict);
        return false;
    }

    if (2 != PyDict_Size(pDict)) {
        return false;
    }

    PyObject *pcode = PyDict_GetItemString(pDict, "code");
    PyObject *pvalue = PyDict_GetItemString(pDict, "data");

    PyArg_Parse(pcode, "i", &code);
    PyArg_Parse(pvalue, "s", &data);

    if(code < 1){
        Py_DECREF(pFunc);
        Py_DECREF(pDict);

        return false;
    }

    Py_DECREF(pFunc);
    Py_DECREF(pDict);
    return true;

}

bool vca_murderHook::startShare(const string &sourceFilePath,
                                const string &sourceSeedPath,
                                const string &localHost) {

    PyObject *pFunc = nullptr;
    PyObject *pDict = nullptr;
    PyObject* args = PyTuple_New(3);

    int code;
    const char *data;

    PyTuple_SetItem(args, 0, Py_BuildValue("s", sourceFilePath.c_str()));
    PyTuple_SetItem(args, 1, Py_BuildValue("s", sourceSeedPath.c_str()));
    PyTuple_SetItem(args, 2, Py_BuildValue("s", localHost.c_str()));

    pFunc = PyObject_GetAttrString(p2p_Module, p2pmthod.startShare);
    if (!pFunc || !PyCallable_Check(pFunc)) {
        Py_DECREF(pFunc);
        return false;
    }

    pDict = PyEval_CallObject(pFunc, args);
    if (nullptr == pDict) {
        Py_DECREF(pDict);
        return false;
    }

    if (2 != PyDict_Size(pDict)) {
        return false;
    }

    PyObject *pcode = PyDict_GetItemString(pDict, "code");
    PyObject *pvalue = PyDict_GetItemString(pDict, "data");

    PyArg_Parse(pcode, "i", &code);
    PyArg_Parse(pvalue, "s", &data);

    if(code < 1){
        Py_DECREF(pFunc);
        Py_DECREF(pDict);

        return false;
    }

    Py_DECREF(pFunc);
    Py_DECREF(pDict);
    return true;
}


bool vca_murderHook::downloadFile(const string &downloadPath,
                                  const string &localSeedPath,
                                  const string &localHost) {

    PyObject *pFunc = nullptr;
    PyObject *pDict = nullptr;
    PyObject* args = PyTuple_New(3);

    int code;
    const char *data;

    PyTuple_SetItem(args, 0, Py_BuildValue("s", downloadPath.c_str()));
    PyTuple_SetItem(args, 1, Py_BuildValue("s", localSeedPath.c_str()));
    PyTuple_SetItem(args, 2, Py_BuildValue("s", localHost.c_str()));

    pFunc = PyObject_GetAttrString(p2p_Module, p2pmthod.downloadFile);
    if (!pFunc || !PyCallable_Check(pFunc)) {
        Py_DECREF(pFunc);
        return false;
    }

    pDict = PyEval_CallObject(pFunc, args);
    if (nullptr == pDict) {
        Py_DECREF(pDict);
        return false;
    }

    if (2 != PyDict_Size(pDict)) {
        return false;
    }

    PyObject *pcode = PyDict_GetItemString(pDict, "code");
    PyObject *pvalue = PyDict_GetItemString(pDict, "data");

    PyArg_Parse(pcode, "i", &code);
    PyArg_Parse(pvalue, "s", &data);

    if(code < 1){
        Py_DECREF(pFunc);
        Py_DECREF(pDict);

        return false;
    }

    Py_DECREF(pFunc);
    Py_DECREF(pDict);
    return true;
}

bool vca_murderHook::stopShare() {
    PyObject *pFunc = nullptr;
    PyObject *pDict = nullptr;
    int code;
    const char *data;

    pFunc = PyObject_GetAttrString(p2p_Module, p2pmthod.stopShare);
    if (!pFunc || !PyCallable_Check(pFunc)) {
        Py_DECREF(pFunc);
        return false;
    }

    pDict = PyEval_CallObject(pFunc, nullptr);
    if (nullptr == pDict) {
        Py_DECREF(pDict);
        return false;
    }

    if (2 != PyDict_Size(pDict)) {
        return false;
    }

    PyObject *pcode = PyDict_GetItemString(pDict, "code");
    PyObject *pvalue = PyDict_GetItemString(pDict, "data");

    PyArg_Parse(pcode, "i", &code);
    PyArg_Parse(pvalue, "s", &data);

    if(code < 1){
        Py_DECREF(pFunc);
        Py_DECREF(pDict);

        return false;
    }

    Py_DECREF(pFunc);
    Py_DECREF(pDict);
    return true;
}