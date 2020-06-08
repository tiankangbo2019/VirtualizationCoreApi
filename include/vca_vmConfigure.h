//
// Created by root on 2019/11/26.
//

#ifndef VIRTUALIZATIONCOREAPI_VCA_VMCONFIGURE_H
#define VIRTUALIZATIONCOREAPI_VCA_VMCONFIGURE_H

#include <iostream>
#include <vector>
#include <map>
#include <algorithm>
#include <stdio.h>
#include <string.h>
#include <string>
#include "libvbox.h"
#include "vca_event.h"
#include <jsoncpp/json/json.h>  //apt-get install libjsoncpp-dev libjsoncpp1

class vcaVmConfigure {//:public Listener

public:
    vcaVmConfigure();  //VBox::COMPtr<VBox::IVirtualBox> vbox

    ~vcaVmConfigure();

    /*************************************************/
    /*
     * 虚拟机处于running时候，不能够添加网卡，但是可以变更网卡模式
     * */
    bool setBridge(const string &_vmname,
                   int slot,
                   const string &hostpci,
                   string &callbackdata);   //设置虚拟机的网卡为桥接模式;

    bool addBridge(const string &_vmname,
                   int slot,
                   const string &hostpci,
                   string &callbackdata);   //添加桥接模式网卡;

    bool setNat(const string &_vmname,
                int slot,
                string &callbackdata);      //设置虚拟机的网卡为网络地址转换（nat);

    bool addNat(const string &_vmname,
                int slot,
                string &callbackdata);      //添加网络地址转换（nat)网卡;

    bool setAdaptorMAC(const string &_vmname,
                       int slot,
                       const string &mac,
                       string &callbackdata);      //为虚拟机设置网卡mac

    bool disableAdaptor(const string &_vmname,
                        int slot,
                        string &callbackdata);      //为虚拟机删除一块网卡

    bool setSimpleRemoteDisplay(const string &_vmname,
                                const string &ip,
                                int port,
                                bool allowMultiConnect,
                                bool reuseSingleConnect,
                                string &callbackdata);    //设置虚拟机RDP

    bool getRdpInfo(const string &vmname,
                    string &vdredata,
                    string &callbackdata);      //获取虚拟机的rdp信息

    bool disableRDP(const string &_vmname, string &callbackdata);     //关闭rdp远程桌面


    bool setRecordingScreenConfig(const string &vmname,
                                  int screenid,
                                  bool on,
                                  const string &basepath,
                                  int width,
                                  int height,
                                  int fps,
                                  int kbps,
                                  int audioquality,
                                  string &videostoragepath,
                                  string &callback);  //设置录屏幕配置

    bool switchRecordingScreen(const string &vmname,
                               int screenid,
                               bool on,
                               string &callback);    //开启/关闭屏幕录制(开启switch_l=true;关闭switch_l=false)

    bool createSharefolder(const string &_vmname,
                           const string &foldername,
                           const string &externalpath,
                           bool writeable,
                           bool automount,
                           const string &internalpath,
                           string &callbackdata);     //为指定虚拟机设置共享文件夹

    bool removeSharedFolder(const string &_vmname,
                            const string &foldername,
                            string &callbackdata);      //删除共享文件夹

    /*****************************************************/

    bool resetCpuCount(const string &_vmname,
                       int cpuCount,
                       string &callbackdata);       //重置虚拟机cpu数量

    bool resetMemSize(const string &_vmname,
                      int memorySize,
                      string &callbackdata);        //重置虚拟机内存大小

    bool resetGraphics(const string &vmname,
                       int ramSize,
                       int monitorCount,
                       const string &cardController,
                       bool enable3D,
                       string &callbackdata);  //设置3D支持

    /*****************************************************/

    bool addUSBController(const string &vmname,
                          const string &usbctlname,
                          const string &usbType,
                          string &callbackdata);   //添加usb控制器

    bool delUSBController(const string &vmname,
                          const string &usbctlname,
                          string &callbackdata);  //移除usb控制器

    bool setAudio(const string &vmname,
                  bool in,
                  bool out,
                  string &callbackdata);    //设置声卡

    bool switchAudio(const string &vmname,
                     bool enable,
                     string &callbackdata);    //声卡开关

    bool addSerialPort(const string &vmname,
                       int slot,
                       string &callbackdata);     //添加串口

    bool delSerialPort(const string &vmname,
                       int slot,
                       string &callbackdata);     //删除串口


private:

    Json::FastWriter writer;
    VBox::COMPtr<VBox::IVirtualBox> vbox_c;
    VBox::COMPtr<VBox::IVirtualBoxClient> vboxClient_c;
    VBox::COMPtr<VBox::IVirtualBoxErrorInfo> errInfo_c;
    std::vector<VBox::COMPtr<VBox::IMachine>, std::allocator<VBox::COMPtr<VBox::IMachine>>> machines_c;

    const std::vector<string> graphicsCtrlType = {"VBoxVGA", "VMSVGA", "VBoxSVGA"};

};


#endif //VIRTUALIZATIONCOREAPI_VCA_VMCONFIGURE_H
