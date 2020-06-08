#ifndef VIRTUALIZATIONCOREAPI_VCA_CORE_H
#define VIRTUALIZATIONCOREAPI_VCA_CORE_H

#include <iostream>
#include <vector>
#include <algorithm>
#include <stdio.h>
#include <string.h>
#include <string>
#include "libvbox.h"
#include "vca_event.h"
#include <jsoncpp/json/json.h>  //apt-get install libjsoncpp-dev libjsoncpp1
#include <map>

using namespace std;

typedef signed char int8_t;

typedef short int int16_t;

typedef int int32_t;

# if __WORDSIZE == 64
typedef long int int64_t;
# else
__extension__
typedef long long int  int64_t;
#endif

class QiVCA : public Listener {

public:
    QiVCA();

    ~QiVCA();

private:
    Json::FastWriter writer;
    VBox::COMPtr<VBox::IVirtualBox> vbox;
    VBox::COMPtr<VBox::IVirtualBoxClient> vboxClient;
    VBox::COMPtr<VBox::IVirtualBoxErrorInfo> errInfo;
    std::vector<VBox::COMPtr<VBox::IMachine>, std::allocator<VBox::COMPtr<VBox::IMachine>>> machines;

    const string storageBus[7] = {"IDE", "SATA", "SCSI", "Floppy", "SAS", "USB", "PCIe"};
    const std::vector<string> mediaType = {"VDI", "VHD", "VMDK"};
    const std::vector<string> osType = {
            "Windows31", "Windows95", "Windows98", "WindowsMe", "WindowsNT3x", "WindowsNT4",
            "Windows2000", "WindowsXP", "WindowsXP_64", "Windows2003", "Windows2003_64",
            "WindowsVista", "WindowsVista_64", "Windows2008", "Windows2008_64",
            "Windows7", "Windows7_64", "Windows8", "Windows8_64", "Windows81", "Windows81_64",
            "Windows2012_64", "Windows10", "Windows10_64", "Windows2016_64",
            "Windows2019_64", "WindowsNT", "WindowsNT_64",
            "Linux22", "Linux24", "Linux24_64", "Linux26", "Linux26_64",
            "ArchLinux", "ArchLinux_64", "Debian", "Debian_64", "Fedora", "Fedora_64",
            "Gentoo", "Gentoo_64", "Mandriva", "Mandriva_64", "Oracle", "Oracle_64",
            "RedHat", "RedHat_64", "OpenSUSE", "OpenSUSE_64", "Turbolinux", "Turbolinux_64",
            "Ubuntu", "Ubuntu_64", "Xandros", "Xandros_64", "Linux", "Linux_64",
            "Solaris", "Solaris_64", "OpenSolaris", "OpenSolaris_64", "Solaris11_64",
            "FreeBSD", "FreeBSD_64", "OpenBSD", "OpenBSD_64", "NetBSD", "NetBSD_64",
            "OS2Warp3", "OS2Warp4", "OS2Warp45", "OS2eCS", "OS21x", "OS2",
            "MacOS", "MacOS_64", "MacOS106", "MacOS106_64", "MacOS107_64", "MacOS108_64",
            "MacOS109_64", "MacOS1010_64", "MacOS1011_64", "MacOS1012_64", "MacOS1013_64",
            "DOS", "Netware", "L4", "QNX", "JRockitVE", "VBoxBS_64"
    };

public:

    /*************************************************/
    bool listVMs(string &vms, string &callbackdata);     //列举本机现有虚拟机

    void startUp(const string &_vmname,
                 const string &ui,
                 int &data,
                 string &message);     //虚拟机开机

    bool shutDown(const string &_vmname, string &callbackdata);    //虚拟机关机

    bool powerOff(const string &_vmname, string &callbackdata);    //虚拟机断电

    bool pause(const string &_vmname, string &callbackdata);       //虚拟机挂起

    bool resume(const string &_vmname, string &callbackdata);      //虚拟机的恢复运行

    bool vmSleep(const string &_vmname, string &callbackdata);     //虚拟机休眠,实际序列化到磁盘

    bool restart(const string &_vmname, string &callbackdata);     //虚拟机重启

    bool getVmInfo(const string &_vmname, string &vminfo, string &callbackdata);   //获取虚拟机的信息

    /*************************************************/
    bool getVmSnapshotInfo(const string &_vmname,
                           string &info,
                           string &callbackdata);    //查找虚拟机的快照信息

    bool takeSnapshot(const string &_vmname,
                      const string &basicString,
                      string &callbackdata);    //虚拟机快照

    bool restoreSnapshot(const string &_vmname,
                         const string &basicString,
                         string &callbackdata);   //恢复虚拟机快照

    bool deleteSnapshot(const string &_vmname,
                        const string &basicString,
                        string &callbackdata);    //删除快照

    bool cloneFullVM(const string &_vmname,
                     const string &basefolder,
                     const string &target_vmname,
                     bool keepmacs,
                     bool keephwuuids,
                     bool keepdiskname,
                     string &callbackdata);       //克隆完整虚拟机

    bool cloneLinkVM(const string &_vmname,
                     const string &basefolder,
                     const string &target_vmname,
                     string &callbackdata);       //克隆链接虚拟机

    bool createVMfromSystemdisk(const string &_vmname,
                                const string &ostypeid,
                                int cpucount,
                                int memorysize,
                                const string &basefolder,
                                const string &diskpath,
                                const string &storagebustype,
                                string &callbackdata);  //从系统盘装机


    bool createVMfromISO(const string &_vmname,
                         const string &ostypeid,
                         int cpucount,
                         int memorysize,
                         const string &basefolder,
                         long int logicdisk_size,
                         const string &isopath,
                         const string &diskformat,
                         const string &storagebustype,
                         bool fix,
                         string &vmuuid,
                         string &callbackdata);    //从iso装机


    bool deleteVM(const string &vmname, int _type, string &callbackdata);    //win linux ....

    bool exportVM(const string &_vmname,
                  bool keepmac,
                  const string &ovafileType,
                  const string &exportpath,
                  string &ovapath,
                  string &callbackdata);     //导出VM到指定路径

    bool importVM(const string &vmname,
                  const string &ovapath,
                  bool keepmac,
                  const string &ostypeid,
                  int cpucount,
                  int memory,
                  bool netup,
                  const string &basefolder,
                  string &callbackdata);   //导入VM
    /*******************extends******************************/
    bool makeFloatDisk(const string &disknameorid,
                       const string &diskformat,
                       const string &basefolder,
                       long int size,
                       string &callbackdata);   //create float disk

    bool addDiskToVm(const string &vmname,
                     const string &disknameorid,
                     const string &diskformat,
                     const string &storagecontroller,
                     const string &basefolder,
                     int slot,
                     string &callbackdata);     //attach float disk to vm

    bool detachDiskFromVm(const string &vmname,
                          int slot,
                          string &callbackdata);    //detach float disk to vm

    bool destroyFloatDiskWithoutAttach(const string &diskuuid,
                                       string &callbackdata);   //destroy float disk

};

#endif // VCA_CORE_H
