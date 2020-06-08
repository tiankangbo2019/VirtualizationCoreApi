//
// Created by root on 2019/11/13.
//g++ monitorHook.cpp -I/usr/include/python2.7 -lpython2.7 -ljsoncpp -std=c++11

#include "vca_monitorHook.h"
#include <list>


Monitor::Monitor() {
    //init
    Py_Initialize();
    assert(Py_IsInitialized());
    const char *strcmd = "sys.path.append('../monitor')";
//    const char *strcmd = "sys.path.append('./')";

    PyRun_SimpleString("import sys");
    PyRun_SimpleString("import os");
    PyRun_SimpleString(strcmd);
    //import模块
    cpupre_Module = PyImport_ImportModule(type.cpuPress);
    assert(cpupre_Module);

    cputmp_Module = PyImport_ImportModule(type.cpuTemp);
    assert(cputmp_Module);

    diskioinfo_Module = PyImport_ImportModule(type.diskIo);
    assert(diskioinfo_Module);

    diskrate_Module = PyImport_ImportModule(type.diskRate);
    assert(diskrate_Module);

    memoryrate_Module = PyImport_ImportModule(type.memoryRate);
    assert(memoryrate_Module);

    swaprate_Module = PyImport_ImportModule(type.swapRate);
    assert(swaprate_Module);

    netcount_Module = PyImport_ImportModule(type.netCount);
    assert(netcount_Module);

    netinfo_Module = PyImport_ImportModule(type.netInfo);
    assert(netinfo_Module);

    rangeload_Module = PyImport_ImportModule(type.rangeLoad);
    assert(rangeload_Module);

    uptime_Module = PyImport_ImportModule(type.uptime);
    assert(uptime_Module);

    hardware_Module = PyImport_ImportModule(type.hardware);
    assert(hardware_Module);
}

Monitor::~Monitor() {

    Py_Finalize();

}

bool Monitor::getHostCpuPressure(const char *method, map<const char *, const char *> &cpupre, char *c) {

    PyObject *pFunc = nullptr;
    PyObject *pDict = nullptr;
    int code;
    const char *value;
    const char *obj;

    pFunc = PyObject_GetAttrString(cpupre_Module, method);
    if (!pFunc || !PyCallable_Check(pFunc)) {
        Py_DECREF(pFunc);
        return false;
    }

    pDict = PyEval_CallObject(pFunc, nullptr);
    if (nullptr == pDict) {
        Py_DECREF(pDict);
        return false;
    }

    if (3 != PyDict_Size(pDict)) {
        return false;
    }

    PyObject *pcode = PyDict_GetItemString(pDict, "code");
    PyObject *pvalue = PyDict_GetItemString(pDict, "value");
    PyObject *pobj = PyDict_GetItemString(pDict, "obj");

    PyArg_Parse(pcode, "i", &code);
    PyArg_Parse(pvalue, "s", &value);
    PyArg_Parse(pobj, "s", &obj);

    char cc[10] = {0};
    sprintf(cc, "%d", code);
    strcpy(c, cc);

    cpupre.insert(pair<const char *, const char *>("code", c));
    cpupre.insert(pair<const char *, const char *>("value", value));
    cpupre.insert(pair<const char *, const char *>("obj", obj));

    Py_DECREF(pFunc);
    Py_DECREF(pDict);
    return true;
}

bool Monitor::getHostCpuTemperature(const char *method, map<const char *, const char *> &cputemp, char *c) {
    PyObject *pFunc = nullptr;
    PyObject *pDict = nullptr;
    int code;
    const char *value;
    const char *obj;

    pFunc = PyObject_GetAttrString(cputmp_Module, method);
    if (!pFunc || !PyCallable_Check(pFunc)) {
        Py_DECREF(pFunc);
        return false;
    }

    pDict = PyEval_CallObject(pFunc, nullptr);
    if (nullptr == pDict) {
        Py_DECREF(pDict);
        return false;
    }

    if (3 != PyDict_Size(pDict)) {
        return false;
    }

    PyObject *pcode = PyDict_GetItemString(pDict, "code");
    PyObject *pvalue = PyDict_GetItemString(pDict, "value");
    PyObject *pobj = PyDict_GetItemString(pDict, "obj");

    PyArg_Parse(pcode, "i", &code);
    PyArg_Parse(pvalue, "s", &value);
    PyArg_Parse(pobj, "s", &obj);

    char cc[10] = {0};
    sprintf(cc, "%d", code);
    strcpy(c, cc);

    cputemp.insert(pair<const char *, const char *>("code", c));
    cputemp.insert(pair<const char *, const char *>("value", value));
    cputemp.insert(pair<const char *, const char *>("obj", obj));

    Py_DECREF(pFunc);
    Py_DECREF(pDict);
    return true;
}

bool Monitor::getHostDiskIoINfo(const char *method, map<const char *, const char *> &io, char *c) {

    PyObject *pFunc = nullptr;
    PyObject *pDict = nullptr;
    int code;
    const char *read;
    const char *write;
    const char *obj;

    pFunc = PyObject_GetAttrString(diskioinfo_Module, method);
    if (!pFunc || !PyCallable_Check(pFunc)) {
        Py_DECREF(pFunc);
        return false;
    }

    pDict = PyEval_CallObject(pFunc, nullptr);
    if (nullptr == pDict) {
        Py_DECREF(pDict);
        return false;
    }

    if (4 != PyDict_Size(pDict)) {
        return false;
    }

    PyObject *pcode = PyDict_GetItemString(pDict, "code");
    PyObject *pread = PyDict_GetItemString(pDict, "read");
    PyObject *pwrite = PyDict_GetItemString(pDict, "write");
    PyObject *pobj = PyDict_GetItemString(pDict, "obj");

    PyArg_Parse(pcode, "i", &code);
    PyArg_Parse(pread, "s", &read);
    PyArg_Parse(pwrite, "s", &write);
    PyArg_Parse(pobj, "s", &obj);

    char cc[10] = {0};
    sprintf(cc, "%d", code);
    strcpy(c, cc);

    io.insert(pair<const char *, const char *>("code", c));
    io.insert(pair<const char *, const char *>("read", read));
    io.insert(pair<const char *, const char *>("write", write));
    io.insert(pair<const char *, const char *>("obj", obj));

    Py_DECREF(pFunc);
    Py_DECREF(pDict);
    return true;
}

bool Monitor::getHostDiskRate(const char *method, map<const char *, const char *> &diskrate, char *c) {

    PyObject *pFunc = nullptr;
    PyObject *pDict = nullptr;
    int code;
    const char *total;
    const char *percent;
    const char *free;
    const char *use;
    const char *obj;

    pFunc = PyObject_GetAttrString(diskrate_Module, method);
    if (!pFunc || !PyCallable_Check(pFunc)) {
        Py_DECREF(pFunc);
        return false;
    }

    pDict = PyEval_CallObject(pFunc, nullptr);
    if (nullptr == pDict) {
        Py_DECREF(pDict);
        return false;
    }

    if (6 != PyDict_Size(pDict)) {
        return false;
    }

    PyObject *pcode = PyDict_GetItemString(pDict, "code");
    PyObject *ptotal = PyDict_GetItemString(pDict, "total");
    PyObject *ppercent = PyDict_GetItemString(pDict, "percent");
    PyObject *pfree = PyDict_GetItemString(pDict, "free");
    PyObject *pused = PyDict_GetItemString(pDict, "used");
    PyObject *pobj = PyDict_GetItemString(pDict, "obj");

    PyArg_Parse(pcode, "i", &code);
    PyArg_Parse(ptotal, "s", &total);
    PyArg_Parse(ppercent, "s", &percent);
    PyArg_Parse(pfree, "s", &free);
    PyArg_Parse(pused, "s", &use);
    PyArg_Parse(pobj, "s", &obj);

    char cc[10] = {0};
    sprintf(cc, "%d", code);
    strcpy(c, cc);

    diskrate.insert(pair<const char *, const char *>("code", c));
    diskrate.insert(pair<const char *, const char *>("total", total));
    diskrate.insert(pair<const char *, const char *>("percent", percent));
    diskrate.insert(pair<const char *, const char *>("free", free));
    diskrate.insert(pair<const char *, const char *>("used", use));
    diskrate.insert(pair<const char *, const char *>("obj", obj));

    Py_DECREF(pFunc);
    Py_DECREF(pDict);
    return true;
}

bool Monitor::getHostSwapRate(const char *method, map<const char *, const char *> &swap, char *c) {

    PyObject *pFunc = nullptr;
    PyObject *pDict = nullptr;
    int code;
    const char *total;
    const char *percent;
    const char *free;
    const char *use;
    const char *obj;

    pFunc = PyObject_GetAttrString(swaprate_Module, method);
    if (!pFunc || !PyCallable_Check(pFunc)) {
        Py_DECREF(pFunc);
        return false;
    }

    pDict = PyEval_CallObject(pFunc, nullptr);
    if (nullptr == pDict) {
        Py_DECREF(pDict);
        return false;
    }

    if (6 != PyDict_Size(pDict)) {
        return false;
    }

    PyObject *pcode = PyDict_GetItemString(pDict, "code");
    PyObject *ptotal = PyDict_GetItemString(pDict, "total");
    PyObject *ppercent = PyDict_GetItemString(pDict, "percent");
    PyObject *pfree = PyDict_GetItemString(pDict, "free");
    PyObject *pused = PyDict_GetItemString(pDict, "used");
    PyObject *pobj = PyDict_GetItemString(pDict, "obj");

    PyArg_Parse(pcode, "i", &code);
    PyArg_Parse(ptotal, "s", &total);
    PyArg_Parse(ppercent, "s", &percent);
    PyArg_Parse(pfree, "s", &free);
    PyArg_Parse(pused, "s", &use);
    PyArg_Parse(pobj, "s", &obj);

    char cc[10] = {0};
    sprintf(cc, "%d", code);
    strcpy(c, cc);

    swap.insert(pair<const char *, const char *>("code", c));
    swap.insert(pair<const char *, const char *>("total", total));
    swap.insert(pair<const char *, const char *>("percent", percent));
    swap.insert(pair<const char *, const char *>("free", free));
    swap.insert(pair<const char *, const char *>("used", use));
    swap.insert(pair<const char *, const char *>("obj", obj));

    Py_DECREF(pFunc);
    Py_DECREF(pDict);
    return true;
}

bool Monitor::getHostMemoryRate(const char *method, map<const char *, const char *> &mem, char *c) {

    PyObject *pFunc = nullptr;
    PyObject *pDict = nullptr;
    int code;
    const char *total;
    const char *percent;
    const char *free;
    const char *use;
    const char *obj;

    pFunc = PyObject_GetAttrString(memoryrate_Module, method);
    if (!pFunc || !PyCallable_Check(pFunc)) {
        Py_DECREF(pFunc);
        return false;
    }

    pDict = PyEval_CallObject(pFunc, nullptr);
    if (nullptr == pDict) {
        Py_DECREF(pDict);
        return false;
    }

    if (6 != PyDict_Size(pDict)) {
        return false;
    }

    PyObject *pcode = PyDict_GetItemString(pDict, "code");
    PyObject *ptotal = PyDict_GetItemString(pDict, "total");
    PyObject *ppercent = PyDict_GetItemString(pDict, "percent");
    PyObject *pfree = PyDict_GetItemString(pDict, "free");
    PyObject *pused = PyDict_GetItemString(pDict, "used");
    PyObject *pobj = PyDict_GetItemString(pDict, "obj");

    PyArg_Parse(pcode, "i", &code);
    PyArg_Parse(ptotal, "s", &total);
    PyArg_Parse(ppercent, "s", &percent);
    PyArg_Parse(pfree, "s", &free);
    PyArg_Parse(pused, "s", &use);

    PyArg_Parse(pobj, "s", &obj);

    char cc[10] = {0};
    sprintf(cc, "%d", code);
    strcpy(c, cc);

    mem.insert(pair<const char *, const char *>("code", c));
    mem.insert(pair<const char *, const char *>("total", total));
    mem.insert(pair<const char *, const char *>("percent", percent));
    mem.insert(pair<const char *, const char *>("free", free));
    mem.insert(pair<const char *, const char *>("used", use));
    mem.insert(pair<const char *, const char *>("obj", obj));

    Py_DECREF(pFunc);
    Py_DECREF(pDict);
    return true;
}

bool Monitor::getHostNetCount(const char *method, map<const char *, const char *> &netc) {
    map<const char *, const char *> netcard;
    PyObject *pFunc = nullptr;
    PyObject *pDict = nullptr;

    pFunc = PyObject_GetAttrString(netcount_Module, method);

    if (!pFunc || !PyCallable_Check(pFunc)) {
        Py_DECREF(pFunc);
        Py_DECREF(pDict);
        return false;
    }

    pDict = PyEval_CallObject(pFunc, nullptr);
    if (PyDict_Size(pDict) < 1) {
        Py_DECREF(pFunc);
        Py_DECREF(pDict);
        return false;
    }

    PyObject *key, *value;
    Py_ssize_t pos = 0;

    while (PyDict_Next(pDict, &pos, &key, &value)) {
        char *c_pstr = PyString_AsString(key);
        char *c_vptr = PyString_AsString(value);

        netc.insert(pair<const char *, const char *>(c_pstr, c_vptr));
    }

    Py_DECREF(pFunc);
    Py_DECREF(pDict);
    return true;
}

bool Monitor::getHostNetInfo(const char *method, map<const char *, const char *> &netinfo, char *c) {
    PyObject *pFunc = nullptr;
    PyObject *pDict = nullptr;
    int code;
    const char *recv;
    const char *send;
    const char *obj;

    pFunc = PyObject_GetAttrString(netinfo_Module, method);
    if (!pFunc || !PyCallable_Check(pFunc)) {
        Py_DECREF(pFunc);
        return false;
    }

    pDict = PyEval_CallObject(pFunc, nullptr);
    if (nullptr == pDict) {
        Py_DECREF(pDict);
        return false;
    }

    if (4 != PyDict_Size(pDict)) {
        return false;
    }

    PyObject *pcode = PyDict_GetItemString(pDict, "code");
    PyObject *precv = PyDict_GetItemString(pDict, "recv");
    PyObject *psend = PyDict_GetItemString(pDict, "send");
    PyObject *pobj = PyDict_GetItemString(pDict, "obj");

    PyArg_Parse(pcode, "i", &code);
    PyArg_Parse(precv, "s", &recv);
    PyArg_Parse(psend, "s", &send);
    PyArg_Parse(pobj, "s", &obj);

    char cc[10] = {0};
    sprintf(cc, "%d", code);
    strcpy(c, cc);

    netinfo.insert(pair<const char *, const char *>("code", c));
    netinfo.insert(pair<const char *, const char *>("recv", recv));
    netinfo.insert(pair<const char *, const char *>("send", send));
    netinfo.insert(pair<const char *, const char *>("obj", obj));

    Py_DECREF(pFunc);
    Py_DECREF(pDict);
    return true;
}

bool Monitor::getHostRangeLoad(const char *method, map<const char *, const char *> &load, char *c) {

    PyObject *pFunc = nullptr;
    PyObject *pDict = nullptr;
    int code;
    const char *value_1;
    const char *value_5;
    const char *value_15;
    const char *obj;

    pFunc = PyObject_GetAttrString(rangeload_Module, method);
    if (!pFunc || !PyCallable_Check(pFunc)) {
        Py_DECREF(pFunc);
        return false;
    }

    pDict = PyEval_CallObject(pFunc, nullptr);
    if (nullptr == pDict) {
        Py_DECREF(pDict);
        return false;
    }

    if (5 != PyDict_Size(pDict)) {
        return false;
    }

    PyObject *pcode = PyDict_GetItemString(pDict, "code");
    PyObject *pvalue_1 = PyDict_GetItemString(pDict, "lavg_1");
    PyObject *pvalue_5 = PyDict_GetItemString(pDict, "lavg_5");
    PyObject *pvalue_15 = PyDict_GetItemString(pDict, "lavg_15");
    PyObject *pobj = PyDict_GetItemString(pDict, "obj");

    PyArg_Parse(pcode, "i", &code);
    PyArg_Parse(pvalue_1, "s", &value_1);
    PyArg_Parse(pvalue_5, "s", &value_5);
    PyArg_Parse(pvalue_15, "s", &value_15);
    PyArg_Parse(pobj, "s", &obj);

    char cc[10] = {0};
    sprintf(cc, "%d", code);
    strcpy(c, cc);

    load.insert(pair<const char *, const char *>("code", c));
    load.insert(pair<const char *, const char *>("lavg_1", value_1));
    load.insert(pair<const char *, const char *>("lavg_5", value_5));
    load.insert(pair<const char *, const char *>("lavg_15", value_15));
    load.insert(pair<const char *, const char *>("obj", obj));

    Py_DECREF(pFunc);
    Py_DECREF(pDict);
    return true;
}

bool Monitor::getHostUptime(const char *method, map<const char *, const char *> &uptime, char *c) {

    PyObject *pFunc = nullptr;
    PyObject *pDict = nullptr;
    int code;
    const char *value;
    const char *obj;

    pFunc = PyObject_GetAttrString(uptime_Module, method);
    if (!pFunc || !PyCallable_Check(pFunc)) {
        Py_DECREF(pFunc);
        return false;
    }

    pDict = PyEval_CallObject(pFunc, nullptr);
    if (nullptr == pDict) {
        Py_DECREF(pDict);
        return false;
    }

    if (3 != PyDict_Size(pDict)) {
        return false;
    }

    PyObject *pcode = PyDict_GetItemString(pDict, "code");
    PyObject *pvalue = PyDict_GetItemString(pDict, "value");
    PyObject *pobj = PyDict_GetItemString(pDict, "obj");

    PyArg_Parse(pcode, "i", &code);
    PyArg_Parse(pvalue, "s", &value);
    PyArg_Parse(pobj, "s", &obj);

    char cc[10] = {0};
    sprintf(cc, "%d", code);
    strcpy(c, cc);

    uptime.insert(pair<const char *, const char *>("code", c));
    uptime.insert(pair<const char *, const char *>("value", value));
    uptime.insert(pair<const char *, const char *>("obj", obj));

    Py_DECREF(pFunc);
    Py_DECREF(pDict);
    return true;
}

/*
 * 获取主机物理信息
 * */
bool Monitor::getHostHardware(const char *method, map<const char *, const char *> &hardware, char *c) {
//    {'kernel': 'Linux 4.4.0-169-generic #198-Ubuntu SMP Tue Nov 12 10:38:00 UTC 2019',
//     'code': 1,
//     'obj': 'node',
//     'cpumodel': 'Intel(R) Core(TM) i3-4160 CPU @ 3.60GHz',
//     'osversion': '4.4.0-169-generic',
//     'cpus': '4',
//     'sockets': '1'}
    PyObject *pFunc = nullptr;
    PyObject *pDict = nullptr;
    int code;
    const char *value;
    const char *obj;
    const char* kernal;
    const char* cpumodel;
    const char* osversion;
    const char* sockets;
    const char* cpus;

    pFunc = PyObject_GetAttrString(hardware_Module, method);
    if (!pFunc || !PyCallable_Check(pFunc)) {
        Py_DECREF(pFunc);
        return false;
    }

    pDict = PyEval_CallObject(pFunc, nullptr);
    if (nullptr == pDict) {
        Py_DECREF(pDict);
        return false;
    }

    if (7 != PyDict_Size(pDict)) {
        return false;
    }

    PyObject *pcode = PyDict_GetItemString(pDict, "code");
    PyObject *pobj = PyDict_GetItemString(pDict, "obj");
    PyObject *pkernal = PyDict_GetItemString(pDict, "kernel");
    PyObject *pcpumodel = PyDict_GetItemString(pDict, "cpumodel");
    PyObject *posversion = PyDict_GetItemString(pDict, "osversion");
    PyObject *psockets = PyDict_GetItemString(pDict, "sockets");
    PyObject *pcpus = PyDict_GetItemString(pDict, "cpus");

    PyArg_Parse(pcode, "i", &code);
    PyArg_Parse(pobj, "s", &obj);
    PyArg_Parse(pkernal, "s", &kernal);
    PyArg_Parse(pcpumodel, "s", &cpumodel);
    PyArg_Parse(posversion, "s", &osversion);
    PyArg_Parse(psockets, "s", &sockets);
    PyArg_Parse(pcpus, "s", &cpus);


    char cc[10] = {0};
    sprintf(cc, "%d", code);
    strcpy(c, cc);

    hardware.insert(pair<const char *, const char *>("code", c));
    hardware.insert(pair<const char *, const char *>("obj", obj));
    hardware.insert(pair<const char *, const char *>("kernel", kernal));
    hardware.insert(pair<const char *, const char *>("cpumodel", cpumodel));
    hardware.insert(pair<const char *, const char *>("osversion", osversion));
    hardware.insert(pair<const char *, const char *>("sockets", sockets));
    hardware.insert(pair<const char *, const char *>("cpus", cpus));

    Py_DECREF(pFunc);
    Py_DECREF(pDict);

    return true;
}

/*
 * 组合主机监控信息
 * */
bool Monitor::getHostInfo(string &hostmes, string &data) {

    Json::Value jsonarray;
    Json::FastWriter writer;

    char c1[10], c2[10], c3[10], c4[10], c5[10], c6[10], c7[10], c8[10], c9[10], c10[10];

    map<const char *, const char *> cpupre;
    map<const char *, const char *> cputemp;
    map<const char *, const char *> diskio;
    map<const char *, const char *> diskrate;
    map<const char *, const char *> memory;
    map<const char *, const char *> netcount;
    map<const char *, const char *> netinfo;
    map<const char *, const char *> avgload;
    map<const char *, const char *> swap;
    map<const char *, const char *> uptime;
    map<const char *, const char *> hardware;

    bool ret_cpupre = getHostCpuPressure(mthod.getCpuRate, cpupre, c1);
    if (ret_cpupre) {
        jsonarray.append(mapTojsonstr(cpupre));
    } else{
        data = "Failed to get host cpu pressure information!";
    }

    bool ret_cputemp = getHostCpuTemperature(mthod.getCpuTemp, cputemp, c2);
    if (ret_cputemp) {
        jsonarray.append(mapTojsonstr(cputemp));

    } else{
        data = "Failed to get host cputemp information!";
    }

    bool ret_io = getHostDiskIoINfo(mthod.getDiskIoInfo, diskio, c3);
    if (ret_io) {
        jsonarray.append(mapTojsonstr(diskio));
    } else{
        data = "Failed to get host io information!";
    }

    bool ret_diskrate = getHostDiskRate(mthod.getDiskRate, diskrate, c4);
    if (ret_diskrate) {
        jsonarray.append(mapTojsonstr(diskrate));
    } else{
        data = "Failed to get host disk information!";
    }

    bool ret_swap = getHostSwapRate(mthod.getSwapRate, swap, c5);
    if (ret_swap) {
        jsonarray.append(mapTojsonstr(swap));
    } else{
        data = "Failed to get host swap information!";
    }

    bool ret_mem = getHostMemoryRate(mthod.getMemoryRate, memory, c6);
    if (ret_mem) {
        jsonarray.append(mapTojsonstr(memory));
    } else{
        data = "Failed to get host memory information!";
    }

    bool ret_netc = getHostNetCount(mthod.getNetCount, netcount);
    if (ret_netc) {
        jsonarray.append(mapTojsonstr(netcount));
    } else{
        data = "Failed to get host network information!";
    }

    bool ret_netinfo = getHostNetInfo(mthod.getNetInfo, netinfo, c7);
    if (ret_netinfo) {
        jsonarray.append(mapTojsonstr(netinfo));
    } else{
        data = "Failed to get host netinfo information!";
    }

    bool ret_load = getHostRangeLoad(mthod.getRangeLoad, avgload, c8);
    if (ret_load) {
        jsonarray.append(mapTojsonstr(avgload));
    } else{
        data = "Failed to get host load information!";
    }

    bool ret_uptime = getHostUptime(mthod.getUptime, uptime, c9);
    if (ret_uptime) {
        jsonarray.append(mapTojsonstr(uptime));
    } else{
        data = "Failed to get host uptime information!";
    }

    bool ret_hardware = getHostHardware(mthod.getHardware, hardware, c9);
    if (ret_hardware) {
        jsonarray.append(mapTojsonstr(hardware));
    } else{
        data = "Failed to get host hardware information!";
    }

    hostmes = writer.write(jsonarray);

    return !hostmes.empty();

}

/*
 * 转换类型：map->json
 * */
string Monitor::mapTojsonstr(map<const char *, const char *> &map_info) {

    Json::Value jObject;

    for (map<const char *, const char *>::const_iterator iter = map_info.begin(); iter != map_info.end(); ++iter) {
        jObject[iter->first] = iter->second;
    }
    return jObject.toStyledString();
}

//int main()
//{
//    string info;
//    Monitor *m = new Monitor();
//    m->getHostInfo(info);
//    std::cout<<"info->"<<info<<endl;
//}