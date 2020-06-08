//
// Created by root on 2019/12/3.
//

#ifndef VIRTUALIZATIONCOREAPI_VCA_ENUMMETHOD_H
#define VIRTUALIZATIONCOREAPI_VCA_ENUMMETHOD_H
/*
 * 每一个枚举为一个rdp接口，而枚举中的值是虚拟机的功能函数接口
 * */
enum vmcommon {
    startUp = 0,    //开机
    shutDown = 1,   //关机
    powerOff = 2,   //断电
    pauses = 3, //挂起
    resume = 4, //恢复
    vmSleep = 5,    //挂起到磁盘
    restart = 6,    //重启
    getVmInfo = 7,  //获取虚拟机信息
    listVMs = 8,    //获取虚拟机列表
    deleteVM = 9,   //删除虚拟机
};

enum vmoperatesnap {
    getVmSnapshotInfo = 10, //获取虚拟机快照信息
    takeSnapshot = 11,  //创建快照
    restoreSnapshot = 12,   //恢复快照
    deleteSnapshot = 13,    //删除快照
};

enum vmclone {
    cloneFullVM = 14,   //完整克隆
    cloneLinkVM = 15,   //链接克隆
};

enum vmexpAndimp {
    exportVM = 16,  //导出vm
    importVM = 17,  //导入vm
};

enum vminstall {
    createVMfromSystemdisk = 18,    //从磁盘装机
    createVMfromISO = 19,   //从iso装机
};

enum vmnetwork {
    setBridge = 20, //设置桥接
    addBridge = 21, //添加桥接
    setNat = 22,    //设置nat
    addNat = 23,    //添加nat
    setAdaptorMAC = 24, //设置mac
    disableAdaptor = 25,    //禁用网卡
};

enum vmrdp {
    setSimpleRemoteDisplay = 26,    //设置rdp
    getRdpInfo = 27,    //获取rdp信息
    disableRDP = 28,    //关闭rdp
};

enum vmrecordscreen {
    setRecordingScreenConfig = 29,  //录屏设置
    switchRecordingScreen = 30, //开关录屏
};

enum vmsharefolder {
    createSharefolder = 31, //创建共享文件夹
    removeSharedFolder = 32,    //移除共享文件夹
};

enum vmconfig {
    resetCpuCount = 33, //重置cpu
    resetMemSize = 34,  //重置内存
    resetGraphics = 35, //重置显卡
};

enum vmusb {
    addUSBController = 36,  //添加usb
    delUSBController = 37,  //删除usb
};

enum vmaudio {
    setAudio = 38,  //添加声卡
    switchAudio = 39,   //禁用声卡
};

enum vmserial {
    addSerialPort = 40, //添加串口
    delSerialPort = 41, //禁用串口
};

enum p2p {
    startTracker = 42,  //开启tracker
    stopTracker = 43,   //关闭tracker
    produceSeed = 44,   //生产种子
    startShare = 45,    //开启分享服务
    downloadFile = 46,  //开始下载
    stopShare = 47,     //停止分享服务
};

enum extends {
    makeFloatDisk = 101,    //制作虚拟机磁盘
    addDiskToVm = 102,  //给虚拟机附加磁盘
    detachDiskFromVm = 103, //detach
    destroyFloatDiskWithoutAttach = 104, // destroy
};

#endif //VIRTUALIZATIONCOREAPI_VCA_ENUMMETHOD_H
