//
// Created by root on 2019/11/13.
//
#ifndef VIRTUALIZATIONCOREAPI_VCA_MONITORHOOK_H
#define VIRTUALIZATIONCOREAPI_VCA_MONITORHOOK_H

#include <iostream>
#include <utility>
#include <python2.7/Python.h>   //-I/usr/include/python2.7 -lpython2.7
#include <map>
#include <string>
#include <jsoncpp/json/json.h>  //apt-get install libjsoncpp-dev libjsoncpp1

using namespace std;

static struct monitorType{
    const char* cpuPress = "cpu_pressure";  //cpu压力
    const char* cpuTemp = "cpu_temperature";    //cpu温度
    const char* diskIo = "disk_io";     //磁盘io
    const char* diskRate = "disk_rate";     //磁盘使用率
    const char* memoryRate = "memory_rate";     //内存使用率
    const char* netCount = "net_count";     //网卡信息
    const char* netInfo = "net_info";       //网络流量
    const char* rangeLoad = "range_load";       //机器负载
    const char* swapRate = "swap_rate";     //磁盘交换区
    const char* uptime = "up_time";     //主机开机时间
    const char* hardware = "hardware";      //主机物理信息

}type;

static struct method{
    const char* getCpuRate = "get_cpurate";
    const char* getCpuTemp = "get_cpuT";
    const char* getDiskIoInfo = "disk_ioinfo";
    const char* getDiskRate = "fun_disk";
    const char* getMemoryRate = "fun_memory";
    const char* getNetCount = "net_count";
    const char* getNetInfo = "net_info";
    const char* getRangeLoad = "get_load";
    const char* getSwapRate = "fun_swap";
    const char* getUptime = "uptime";
    const char* getHardware = "hardware_message";

}mthod;

class Monitor
        {
public:
            Monitor();
            ~Monitor();

            bool getHostInfo(string &hostmes, string &data);
private:
            /*cpp中map转化为json*/
            static string mapTojsonstr(map<const char *, const char *>& map_info);

            /*获取主机cpu压力*/
            PyObject* cpupre_Module;
            bool getHostCpuPressure(const char* method, map<const char*, const char *> &, char*);
            /*获取主机cpu温度*/
            PyObject* cputmp_Module;
            bool getHostCpuTemperature(const char* method, map<const char*, const char *> &, char*);
            /*获取主机磁盘io读写*/
            PyObject* diskioinfo_Module;
            bool getHostDiskIoINfo(const char* method, map<const char*, const char *> &, char*);
            /*获取主机磁盘使用率*/
            PyObject* diskrate_Module;
            bool getHostDiskRate(const char* method, map<const char*, const char *> &, char*);
            /*获取主机的内存使用信息*/
            PyObject* memoryrate_Module;
            bool getHostMemoryRate(const char* method, map<const char*, const char *> &, char*);
            /*获取主机的磁盘交换空间使用信息*/
            PyObject* swaprate_Module;
            bool getHostSwapRate(const char* method, map<const char*, const char *> &, char*);
            /*获取主机的主要网卡*/
            PyObject* netcount_Module;
            bool getHostNetCount(const char* method, map <const char*, const char*> &);
            /*获取主机的总上行下行流量*/
            PyObject* netinfo_Module;
            bool getHostNetInfo(const char* method, map<const char*, const char *> &, char*);
            /*获取主机的机器负载*/
            PyObject* rangeload_Module;
            bool getHostRangeLoad(const char* method, map<const char*, const char *> &, char*);
            /*获取主机的开机时间*/
            PyObject* uptime_Module;
            bool getHostUptime(const char* method, map<const char*, const char *> &, char*);
            /*获取主机的物理信息*/
            PyObject* hardware_Module;
            bool getHostHardware(const char* method, map<const char*, const char *> &, char*);
        };


#endif //VIRTUALIZATIONCOREAPI_VCA_MONITORHOOK_H
