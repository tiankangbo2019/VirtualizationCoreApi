//
//#include <iostream>
//#include "../include/vca_core.h"
//#include "../include/vca_vmConfigure.h"
//
//
//using namespace std;
//
//int main() {
//
//    QiVCA *w = new QiVCA();
//
///*
// * 1.获取虚拟机列表
// * */
////    string lists;
////    w->listVMs(lists);
////    cout << "listvms->" << lists << endl;
//
///*
// * 2.虚拟机开机
// * */
////    w->startUp("centos01-1", "headless");
//
///*
// * 3.获取主机信息
// * */
////    string info;
////    w->getHostInfo(info);
////    cout << "host-->" << info << endl;
///*
// * 4.虚拟机关机
// * */
////    w->shutDown("centos01");
//
///*
// *5.虚拟机断电
// * */
////    w->powerOff("centos01");
//
///*
// * 6.虚拟机挂起
// * */
////    w->pause("centos01");
//
///*
// * 7.虚拟机恢复运行
// * */
////    w->resume("centos01");
//
///*
// * 8.虚拟机休眠
// * */
////    w->vmSleep("centos01-1");
//
///*
// * 9.虚拟机重启
// * */
////    bool ret = w->restart("centos01-1");
////    cout << ret << endl;
//
///*
// * 10.获取虚拟机快照信息
// * */
////    string snaps;
////    w->getVmSnapshotInfo("centos01-1", snaps);
////    cout << "snap->" << snaps << endl;
//
///*
// * 11.创建虚拟机快照
// * */
//
////    w->takeSnapshot("centos01-1", "snapshot_two");
//
///*
// * 12.恢复快照
// * */
////    w->restoreSnapshot("centos01-1", "test");
//
///*
// * 13.删除快照
// * */
////    w->deleteSnapshot("centos01-1", "snapshot_two");
//
///*
// * 14.获取虚拟机信息
// * */
////    string data;
////    bool ret_1 = w->getVmInfo("centos01-1", data);
////    std::cout << data << endl;
//
///*
// * 15.完整克隆虚拟机
// * */
////    w->cloneFullVM("centos01", "/root/img", "targetmachine2", false, false, false);
//
///*
// * 16.链接克隆虚拟机
// * */
////    w->cloneLinkVM("centos01", "/root/img", "targetmachine1_4");
//
//
///*
// * 17.设置虚拟机桥接网卡
// * */
////    w->setBridge("targetmachine1_2", 0, "tap100"); //bridge
//
///*
// * 18.设置虚拟机网络地址NAT
// * */
////        w->setNat("targetmachine1_2", 1);
//
///*
// * 19.关闭虚拟机网卡
// * */
////    w->disableAdaptor("targetmachine1_2", 0);
//
//
///*
// * 20.设置网卡mac
// * */
////    w->setAdaptorMAC("targetmachine1_2", 0, "080027DC71A2");
//
///*
// * 21.创建共享文件夹
// * */
////    w->createSharefolder("targetmachine1_2", "my", "/mnt", true, true, "/opt");
//
///*
// * 22.删除共享文件夹
// * */
//
////    w->removeSharedFolder("targetmachine1_2", "img");
//
///*
// * 23.从系统盘disk安装虚拟机
// * */
///*
// * 从images创建虚拟机,至少传入参数:
// * 虚拟机名字;
// * 系统类型ID;
// * 初始cpu数量;
// * 初始内存大小MB;
// * 系统盘全路径;
// * 磁盘接口类型;
// * */
////    w->createVMfromSystemdisk("mycentos_test", "RedHat_64", 1, 1024, "/root/img/centos.img", "SATA");
//
///*
// * 24.从iso创建
// * */
///*
// * 从ISO进行装机，至少传入参数：
// * 虚拟机名字;
// * 系统类型ID;
// * 初始CPU数量;
// * 初始内存大小MB;
// * 装机磁盘本地路径;
// * 创建逻辑磁盘大小;
// * ISO全路径;
// * 媒体设备接口类型;
// * */
////    long int size = 1024*1024*1024L*10;
////
////    string path, info;
////    bool a = w->createVMfromISO("RedHat1", "RedHat_64", 1, 1024,"/root/img", size,
////            "/root/img/CentOS-7.iso", "VDI", "SATA", false, path, info);
////
////    cout<<"vm path :"<<path<<endl;
//
///*
// * 25.删除vm
// *      UnregisterOnly = 1,
//        DetachAllReturnNone = 2,
//        DetachAllReturnHardDisksOnly = 3,
//        Full = 4,
// * */
////    w->deleteVM("RedHat", 4);
//
///*
// * 26.导出虚拟机
// * */
////    std::string ovapath;
////    bool state = w->exportVM("centos01 1", true, "ovf-2.0", "/root", ovapath);
////    cout << "ovapath->"<<ovapath << endl;
//
///*
// * 27.导入虚拟机
//
////    bool state = w->importVM("/root/centos02.ova", false, "RedHat_64",
////                             "rehat7.1",
////                             "1", "1024", true,
////                             "/root/img");
////
////    cout << "state->" << state << endl;
//
////    w->getMesFromRdp();
//
//
///////////////////////////////////
////    vca_vmConfigure *vca_c = new vca_vmConfigure(); //VBox::COMPtr<VBox::IVirtualBox>()
////    vca_c->setNat("centos01", 0);
///*
// * 获取简单rdp的链接信息
// * */
////    string vmvrde;
////
////    vca_c->getRdpInfo("centos01", vmvrde);
////    std::cout << "vm vrde info->" << vmvrde << endl;
//
///*
// * 设置简单rdp信息
// * */
//
////    bool ret = vca_c->setSimpleRemoteDisplay("rehat7.1", "0.0.0.0", 5906, true, false);
////    std::cout << "ret->" << ret << endl;
//
///*
// * 设置桥接网卡
// * */
////    vca_c->setBridge("rehat7.1", 1, "p0");
//
///*
// * 关闭虚拟机远程桌面
// * */
////    vca_c->disableRDP("centos01 1");
//
///*
// * 重置虚拟机cpu数量
// * */
////    vca_c->resetCpuCount("centos01 1", 2);
//
///*
// * 设置显卡
// * */
////    vca_c->resetGraphics("centos01 1", 32, 2, "VBoxSVGA", true);
//
///*
// * 设置录制屏幕
// * */
////    string videostoragepath;
////    vca_c->setRecordingScreenConfig(false,
////                                    "centos01 1",
////                                    0,
////                                    "",
////                                    videostoragepath,
////                                    1024,
////                                    768,
////                                    30,
////                                    1024,
////                                    0);
////
////    std::cout<<"videostoragepath->"<<videostoragepath<<endl;
//
///*
// * 生效录制
// * */
////    vca_c->switchRecordingScreen("centos01 1", 0, false);
//
///*
// * 设置声卡
// * */
////    vca_c->setAudio("centos-001-1", true, true);
//
///*
// * 开关声卡
// * */
////    vca_c->switchAudio("centos-001-1", true);
//
///*
// * 添加usb控制器
// * */
////    vca_c->addUSBController("centos-001-1", "usb","XHCI");
//
///*
// * 移除USB控制器
// * */
////    vca_c->delUSBController("centos-001-1", "usb");
//
////    vca_c->addSerialPort("centos-001-1", 0);
//
////    vca_c->delSerialPort("centos-001-1", 0);
//
//
//
//    return 0;
//}