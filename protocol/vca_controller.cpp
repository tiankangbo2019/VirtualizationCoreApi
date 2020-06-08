//
// Created by root on 2019/12/3.
//
#include "vca_controller.h"
#include "vca_enummethod.h"

#ifdef __cplusplus
extern "C" {
#endif

#include <unistd.h>

static pid_t getProcessPidByName(const char *proc_name) {
    FILE *fp;
    char buf[100];
    char cmd[200] = {'\0'};
    pid_t pid = -1;
    sprintf(cmd, "pidof %s", proc_name);

    if ((fp = popen(cmd, "r")) != NULL) {
        if (fgets(buf, 255, fp) != NULL) {
            pid = atoi(buf);
        }
    }
    pclose(fp);
    return pid;
}

#ifdef __cplusplus
}
#endif

unsigned long vca_controller::get_tid() {
    std::thread::id tid = std::this_thread::get_id();
    std::ostrstream os;
    os << tid;
    unsigned long tidx = std::stol(os.str());
    return tidx;
}

grpc::Status vca_controller::Test(grpc::ServerContext *context, const Qigrpc::requestNull *request,
                                  Qigrpc::vcaReply *response) {
    printf("%s %d\n", __func__, __LINE__);
    /*
     * VBoxXPCOMIPCD
     *
     * VBoxSVC
     * */
    pid_t xpcom_process_pid = getProcessPidByName("VBoxXPCOMIPCD");
    pid_t svc_process_pid = getProcessPidByName("VBoxSVC");
    int code = 0;
    if (xpcom_process_pid && svc_process_pid) {
        code = 1;
    }
    response->set_vcamethod("Test");
    response->set_code(code);
    response->set_data("VirtualizationCoreAPI Version 1.0.0");
    return grpc::Status::OK;
}

grpc::Status vca_controller::VmCommon(grpc::ServerContext *context, const Qigrpc::requestSimple *request,
                                      Qigrpc::vcaReply *response) {
    printf("%s %d\n", __func__, __LINE__);
    int method_id = request->vcamethod();
    auto *common = new QiVCA();

    bool ret = false;
    string vminfo;
    string vmlist;

    switch (method_id) {
        case startUp : {
            int data;
            string message;
            const string &vmname = request->vmname();

            std::cout << "调用开机接口" << method_id << endl;
            common->startUp(vmname, "headless", data, message);
            response->set_vcamethod("startUp");

            if (data == 1) {
                ret = true;
            } else {
                ret = false;
                response->set_data(message);
            }
            response->set_code(ret);
        }
            break;

        case shutDown: {
            string data;
            const string &vmname = request->vmname();

            std::cout << "调用关机接口" << method_id << endl;
            ret = common->shutDown(vmname, data);
            response->set_vcamethod("shutDown");
            response->set_data(data);
            response->set_code(ret);
        }
            break;

        case powerOff: {
            string data;
            const string &vmname = request->vmname();

            std::cout << "调用断电接口" << method_id << endl;
            ret = common->powerOff(vmname, data);
            response->set_vcamethod("powerOff");
            response->set_data(data);
            response->set_code(ret);
        }
            break;

        case pauses: {
            string data;
            const string &vmname = request->vmname();

            std::cout << "调用挂起接口" << method_id << endl;
            ret = common->pause(vmname, data);
            response->set_vcamethod("pause");
            response->set_data(data);
            response->set_code(ret);
        }
            break;

        case resume: {
            string data;
            const string &vmname = request->vmname();

            std::cout << "调用恢复运行接口" << method_id << endl;
            ret = common->resume(vmname, data);
            response->set_vcamethod("resume");
            response->set_data(data);
            response->set_code(ret);
        }
            break;

        case vmSleep: {
            string data;
            const string &vmname = request->vmname();

            std::cout << "调用休眠接口" << method_id << endl;
            ret = common->vmSleep(vmname, data);
            response->set_vcamethod("vmSleep");
            response->set_data(data);
            response->set_code(ret);
        }
            break;

        case restart: {
            string data;
            const string &vmname = request->vmname();

            std::cout << "调用重启接口" << method_id << endl;
            ret = common->restart(vmname, data);
            response->set_vcamethod("restart");
            response->set_data(data);
            response->set_code(ret);
        }
            break;

        case getVmInfo: {
            string data;
            const string &vmname = request->vmname();

            std::cout << "调用获取虚拟机信息接口" << method_id << endl;
            ret = common->getVmInfo(vmname, vminfo, data);
            response->set_vcamethod("getVmInfo");
            response->set_code(ret);

            if (ret) {
                response->set_data(vminfo);
            } else {
                response->set_data(data);
            }
        }
            break;

        case listVMs: {
            string data;
            std::cout << "调用获取虚拟机列表接口" << method_id << endl;
            ret = common->listVMs(vmlist, data);
            response->set_vcamethod("listVMs");
            response->set_code(ret);

            if (ret) {
                response->set_data(vmlist);

            } else {
                response->set_data(data);
            }
        }
            break;

        case deleteVM: {
            string data;
            const string &vmname = request->vmname();

            std::cout << "调用删除虚拟机接口" << method_id << endl;
            ret = common->deleteVM(vmname, 4, data);
            response->set_vcamethod("deleteVM");
            response->set_data(data);
            response->set_code(ret);
        }
            break;
        default:
            std::cout << "不存在的接口" << method_id << endl;
            response->set_vcamethod("null");
            response->set_data("null");
            response->set_code(0);
    }

    std::cout << "调用结果" << ret << endl;

    return grpc::Status::OK;
}

// 获取主机信息
grpc::Status vca_controller::HostInfo(grpc::ServerContext *context, const Qigrpc::requestNull *request,
                                      Qigrpc::vcaReply *response) {
    printf("%s %d\n", __func__, __LINE__);

    string info;
    string data;
    bool ret;
    auto *monitor = new Monitor();
    ret = monitor->getHostInfo(info, data);

    response->set_vcamethod("getHostInfo");
    response->set_code(ret);

    if (ret) {
        /*
         * 调用结果为true
         * */
        response->set_data(info);
    } else {
        /*调用结果为false*/
        response->set_data(data);
    }

    std::cout << "调用结果" << ret << endl;

    return grpc::Status::OK;
}

grpc::Status vca_controller::VmInstall(grpc::ServerContext *context, const Qigrpc::requestInstall *request,
                                       Qigrpc::vcaReply *response) {
    printf("%s %d\n", __func__, __LINE__);

    bool ret = false;
    int method_id = request->vcamethod();
    const string &vmname_ = request->vmname();  //虚拟机名字
    const string &ostype_ = request->ostypeid();    //虚拟机系统类型
    int cpucount_ = request->cpucount();    //初始cpu
    int memorysize_ = request->memorysize();    //初始内存
    const string &basefolder_ = request->basefolder();  //虚拟机存储基础路径
    const string &sourcepath_ = request->sourcepath();  //iso/disk资源路径
    const string &storagebustype_ = request->storagebustype();  //虚拟机磁盘接口类型

    auto *install = new QiVCA();

    switch (method_id) {
        case createVMfromISO: {
            bool fix_ = request->fix(); //是否固定磁盘大小
            const string &diskformattype_ = request->diskformat();    //虚拟机磁盘格式化类型(VDI,VHD, VMDK)
            long int logicdisksize_ = request->logicdisksize();
            string vmuuid_;
            string callbackdata;

            ret = install->createVMfromISO(vmname_,/*虚拟机名字*/
                                           ostype_,/*虚拟机系统类型*/
                                           cpucount_,/*初始cpu*/
                                           memorysize_,/*初始内存*/
                                           basefolder_,/*基础路径*/
                                           logicdisksize_,/*磁盘大小建议32GB*/
                                           sourcepath_,/*iso或系统盘资源路径*/
                                           diskformattype_,/*磁盘格式化类型VDI;VHD;VMDK*/
                                           storagebustype_,/*磁盘控制器接口类型*/
                                           fix_,/*是否固定磁盘大小*/
                                           vmuuid_,/*vm uuid*/
                                           callbackdata);/*错误返回*/
            std::cout << "调用createVMfromISO装机方法" << endl;
            response->set_vcamethod("createVMfromISO");
            response->set_code(ret);
            if (ret) {
                response->set_data(vmuuid_);
            } else {
                response->set_data(callbackdata);
            }
        }
            break;

        case createVMfromSystemdisk: {
            string callbackdata;

            ret = install->createVMfromSystemdisk(vmname_,/*虚拟机名字*/
                                                  ostype_,/*虚拟机系统类型*/
                                                  cpucount_,/*初始cpu*/
                                                  memorysize_,/*初始内存*/
                                                  basefolder_,/*基础路径*/
                                                  sourcepath_,/*iso或系统盘资源路径*/
                                                  storagebustype_,/*磁盘控制器接口类型*/
                                                  callbackdata);/*data返回*/
            std::cout << "调用createVMfromSystemdisk装机方法" << endl;
            response->set_vcamethod("createVMfromSystemdisk");
            response->set_code(ret);
            response->set_data(callbackdata);

        }
            break;

        default:
            std::cout << "不存在的接口" << method_id << endl;
            response->set_vcamethod("null");
            response->set_data("null");
            response->set_code(0);

    }
    std::cout << "调用结果" << ret << endl;

    return grpc::Status::OK;
}

grpc::Status vca_controller::VmOperateSnapshot(grpc::ServerContext *context, const Qigrpc::requestSnapshot *request,
                                               Qigrpc::vcaReply *response) {

    printf("%s %d\n", __func__, __LINE__);

    bool ret = false;
    int method_id = request->vcamethod();
    const string &vmname_ = request->vmname();

    auto *snap = new QiVCA();

    switch (method_id) {
        case getVmSnapshotInfo: {
            string snapinfo;
            string callback_;
            ret = snap->getVmSnapshotInfo(vmname_,/*虚拟机名字*/
                                          snapinfo,/*返回信息*/
                                          callback_);
            response->set_vcamethod("getVmSnapshotInfo");
            response->set_code(ret);
            if (ret) {
                response->set_data(snapinfo);
            } else {
                response->set_data(callback_);
            }
            std::cout << "调用getVmSnapshotInfo方法" << endl;
        }
            break;

        case takeSnapshot: {
            string callback_;
            const string &snapshotname_ = request->snapshotname();
            ret = snap->takeSnapshot(vmname_,/*虚拟机名字*/
                                     snapshotname_,/*新快照名字*/
                                     callback_);/*错误返回*/

            response->set_vcamethod("takeSnapshot");
            response->set_code(ret);
            response->set_data(callback_);
            std::cout << "调用takeSnapshot方法" << endl;

        }
            break;

        case restoreSnapshot: {
            string callback_;
            const string &snapshotname_ = request->snapshotname();

            ret = snap->restoreSnapshot(vmname_,/*虚拟机名字*/
                                        snapshotname_,/*快照名字*/
                                        callback_);

            response->set_vcamethod("restoreSnapshot");
            response->set_code(ret);
            response->set_data(callback_);

            std::cout << "调用restoreSnapshot方法" << endl;
        }
            break;

        case deleteSnapshot: {
            string callback_;
            const string &snapshotname_ = request->snapshotname();

            ret = snap->deleteSnapshot(vmname_,/*虚拟机名字*/
                                       snapshotname_,/*快照名字*/
                                       callback_);
            response->set_vcamethod("deleteSnapshot");
            response->set_code(ret);
            response->set_data(callback_);

            std::cout << "调用deleteSnapshot方法" << endl;
        }
            break;
    }

    return grpc::Status::OK;
}

grpc::Status vca_controller::VmClone(grpc::ServerContext *context, const Qigrpc::requestClone *request,
                                     Qigrpc::vcaReply *response) {

    printf("%s %d\n", __func__, __LINE__);

    bool ret = false;
    int method_id = request->vcamethod();
    const string &vmname_ = request->vmname();
    const string &basefolder_ = request->basefolder();
    const string &targetvmname_ = request->targetvmname();

    auto *clone = new QiVCA();

    switch (method_id) {
        case cloneFullVM: {
            string callback_;
            bool keepmac_ = request->keepmacs();
            bool keephwuuid_ = request->keephwuuids();
            bool keepdiskname_ = request->keepdiskname();

            ret = clone->cloneFullVM(vmname_,/*虚拟机名字*/
                                     basefolder_,/*基础路径*/
                                     targetvmname_,/*新虚拟机名字*/
                                     keepmac_,/*保留mac*/
                                     keephwuuid_,/*保留硬件uuid*/
                                     keepdiskname_,/*保留磁盘*/
                                     callback_);
            std::cout << "调用cloneFullVM方法" << endl;
            response->set_vcamethod("cloneFullVM");
            response->set_code(ret);
            response->set_data(callback_);

        }
            break;

        case cloneLinkVM: {
            string callback_;

            ret = clone->cloneLinkVM(vmname_,/*虚拟机名字*/
                                     basefolder_,/*基础路径*/
                                     targetvmname_,/*新虚拟机名字*/
                                     callback_);
            std::cout << "调用cloneLinkVM方法" << endl;
            response->set_vcamethod("cloneLinkVM");
            response->set_code(ret);
            response->set_data(callback_);
        }
            break;
    }

    return grpc::Status::OK;
}

grpc::Status vca_controller::VmImpAndExp(grpc::ServerContext *context, const Qigrpc::requestImpAndexportVM *request,
                                         Qigrpc::vcaReply *response) {

    printf("%s %d\n", __func__, __LINE__);

    bool ret = false;
    int method_id = request->vcamethod();
    const string &vmname_ = request->vmname();
    bool keepmac_ = request->keepmac();

    auto *impaexp = new QiVCA();


    switch (method_id) {
        case exportVM: {
            string callback_;
            string ovapath;
            /*
             * ova file type <"ovf-2.0", "ovf-1.0", "ovf-0.9">
             * */
            string ovafiletype_ = "ovf-2.0";
            const string &exportpath_ = request->exportpath();
            ret = impaexp->exportVM(vmname_,/*虚拟机名字*/
                                    keepmac_,/*保存mac*/
                                    ovafiletype_,/*ovs文件格式*/
                                    exportpath_,/*导出基础路径*/
                                    ovapath,/*导出成功后返回文件路径*/
                                    callback_);
            std::cout << "调用exportVM方法" << endl;
            response->set_vcamethod("exportVM");
            response->set_code(ret);

            if (ret) {
                response->set_data(ovapath);
            } else {
                response->set_data(callback_);
            }
        }
            break;

        case importVM: {
            string callback_;
            const string &ostypeid_ = request->ostypeid();
            const string &ovapath_ = request->ovapath();
            int cpucount_ = request->cpucount();
            int memory_ = request->memory();
            bool netup_ = request->netup();
            const string &basefolder_ = request->basefolder();

            ret = impaexp->importVM(vmname_,/*新虚拟机名字*/
                                    ovapath_,/*ova磁盘路径*/
                                    keepmac_,/*保存mac*/
                                    ostypeid_,/*虚拟机系统类型*/
                                    cpucount_,/*初始cpu*/
                                    memory_,/*初始内存*/
                                    netup_,/*是否保留网卡<一般false，除非两台物理主机上网卡环境一样>*/
                                    basefolder_,/*虚拟机存储基础路径*/
                                    callback_);
            std::cout << "调用importVM方法" << endl;
            response->set_vcamethod("importVM");
            response->set_code(ret);
            response->set_data(callback_);

        }
            break;

    }

    return grpc::Status::OK;
}

grpc::Status vca_controller::VmNetwork(grpc::ServerContext *context, const Qigrpc::requestNetwork *request,
                                       Qigrpc::vcaReply *response) {

    printf("%s %d\n", __func__, __LINE__);

    bool ret = false;
    int method_id = request->vcamethod();
    const string &vmname_ = request->vmname();
    int slot_ = request->slot();
    auto *net = new vcaVmConfigure();

    switch (method_id) {
        case setBridge: {
            string callback_;
            const string &hostpci_ = request->hostpciname();

            ret = net->setBridge(vmname_,/*虚拟机名字*/
                                 slot_,/*slot号码*/
                                 hostpci_,/*主机网卡名字*/
                                 callback_);
            std::cout << "调用setBridge方法" << endl;
            response->set_vcamethod("setBridge");
            response->set_code(ret);
            response->set_data(callback_);

        }
            break;

        case addBridge: {
            string callback_;
            const string &hostpci_ = request->hostpciname();

            ret = net->addBridge(vmname_,/*虚拟机名字*/
                                 slot_,/*slot号码*/
                                 hostpci_,/*主机网卡名字*/
                                 callback_);
            std::cout << "调用addBridge方法" << endl;
            response->set_vcamethod("addBridge");
            response->set_code(ret);
            response->set_data(callback_);
        }
            break;

        case setNat: {
            string callback_;
            ret = net->setNat(vmname_,/*虚拟机名字*/
                              slot_,/*slot号码*/
                              callback_);
            std::cout << "调用setNat方法" << endl;
            response->set_vcamethod("setNat");
            response->set_code(ret);
            response->set_data(callback_);
        }
            break;

        case addNat: {
            string callback_;

            ret = net->addNat(vmname_,/*虚拟机名字*/
                              slot_,/*slot号码*/
                              callback_);
            std::cout << "调用addNat方法" << endl;
            response->set_vcamethod("addNat");
            response->set_code(ret);
            response->set_data(callback_);
        }
            break;

        case setAdaptorMAC: {
            string callback_;
            const string &mac_ = request->mac();

            ret = net->setAdaptorMAC(vmname_,/*虚拟机名字*/
                                     slot_,/*slot号码*/
                                     mac_,/*新mac地址*/
                                     callback_);
            std::cout << "调用setAdaptorMAC方法" << endl;
            response->set_vcamethod("setAdaptorMAC");
            response->set_code(ret);
            response->set_data(callback_);
        }
            break;

        case disableAdaptor: {
            string callback_;

            ret = net->disableAdaptor(vmname_,/*虚拟机名字*/
                                      slot_,/*slot号码*/
                                      callback_);
            std::cout << "调用disableAdaptor方法" << endl;
            response->set_vcamethod("disableAdaptor");
            response->set_code(ret);
            response->set_data(callback_);
        }
            break;

    }

    return grpc::Status::OK;

}

grpc::Status vca_controller::VmRdp(grpc::ServerContext *context, const Qigrpc::requestRdp *request,
                                   Qigrpc::vcaReply *response) {


    printf("%s %d\n", __func__, __LINE__);

    bool ret = false;
    int method_id = request->vcamethod();
    const string &vmname_ = request->vmname();
    auto *rdp = new vcaVmConfigure();
    switch (method_id) {
        case setSimpleRemoteDisplay: {
            string callback_;
            const string &address_ = request->listenaddress();
            int port_ = request->listenport();
            bool allowmulticonnect_ = request->allowmulticonnect();
            bool reusesingleconnect_ = request->reusesingleconnect();
            /*
             * 注意设置端口时候，底层并不自动检测端口使用情况;
             * */
            ret = rdp->setSimpleRemoteDisplay(vmname_,/*虚拟机名字*/
                                              address_,/*监听地址*/
                                              port_,/*监听端口*/
                                              allowmulticonnect_,/*是否允许多链接*/
                                              reusesingleconnect_,/*是否重用单链接*/
                                              callback_);
            std::cout << "调用setSimpleRemoteDisplay方法" << endl;
            response->set_vcamethod("setSimpleRemoteDisplay");
            response->set_code(ret);
            response->set_data(callback_);
        }
            break;

        case getRdpInfo: {
            string callback_;
            string vdredata_;
            ret = rdp->getRdpInfo(vmname_,/*虚拟机名字*/
                                  vdredata_,/*rdp链接数据信息*/
                                  callback_);
            std::cout << "调用getRdpInfo方法" << endl;

            response->set_vcamethod("getRdpInfo");
            response->set_code(ret);
            if (ret) {
                response->set_data(vdredata_);
            } else {
                response->set_data(callback_);
            }
        }
            break;

        case disableRDP: {
            string callback_;
            ret = rdp->disableRDP(vmname_,/*虚拟机名字*/
                                  callback_);
            std::cout << "调用disableRDP方法" << endl;
            response->set_vcamethod("disableRDP");
            response->set_code(ret);
            response->set_data(callback_);
        }
            break;
    }

    return grpc::Status::OK;
}

grpc::Status vca_controller::VmRecord(grpc::ServerContext *context, const Qigrpc::requestRecord *request,
                                      Qigrpc::vcaReply *response) {

    printf("%s %d\n", __func__, __LINE__);

    bool ret = false;
    int method_id = request->vcamethod();
    const string &vmname_ = request->vmname();
    int screenid_ = request->screenid();
    bool switch_ = request->on();
    auto *record = new vcaVmConfigure();

    switch (method_id) {
        case setRecordingScreenConfig: {
            string callback_;
            string videopath_;
            const string &basepath_ = request->basefolder();
            int width_ = request->width();
            int height_ = request->height();
            int fps_ = request->fps();
            int kbps_ = request->kbps();
            int audioquality = 2;   //0, 1, 2

            ret = record->setRecordingScreenConfig(vmname_,/*虚拟机名字*/
                                                   screenid_,/*屏幕id*/
                                                   switch_,/*开关*/
                                                   basepath_,/*视频存储基本路径*/
                                                   width_,/*屏幕宽*/
                                                   height_,/*屏幕高*/
                                                   fps_,/*帧率*/
                                                   kbps_,/*比特率*/
                                                   audioquality,/*音质*/
                                                   videopath_,/*视频文件存储全路径*/
                                                   callback_);
            std::cout << "调用setRecordingScreenConfig方法" << endl;
            response->set_vcamethod("setRecordingScreenConfig");
            response->set_code(ret);

            if (ret) {
                response->set_data(videopath_);
            } else {
                response->set_data(callback_);
            }

        }
            break;

        case switchRecordingScreen: {
            string callback_;

            ret = record->switchRecordingScreen(vmname_,/*虚拟机名字*/
                                                screenid_,/*虚拟机屏幕id*/
                                                switch_,/*开关*/
                                                callback_);
            std::cout << "调用switchRecordingScreen方法" << endl;
            response->set_vcamethod("switchRecordingScreen");
            response->set_code(ret);
            response->set_data(callback_);

        }
            break;

    }

    return grpc::Status::OK;
}

grpc::Status vca_controller::VmSharefolder(grpc::ServerContext *context, const Qigrpc::requestSharefolder *request,
                                           Qigrpc::vcaReply *response) {

    printf("%s %d\n", __func__, __LINE__);

    bool ret = false;
    int method_id = request->vcamethod();
    const string &vmname_ = request->vmname();
    const string &foldername_ = request->sharefoldername();
    auto *share = new vcaVmConfigure();

    switch (method_id) {
        case createSharefolder: {
            string callback_;
            const string &serverpath_ = request->serverpath();
            bool write_ = request->writeable();
            bool automount_ = true;
            const string& tmppath = request->internalpath();
            string internalpath_;
            if(!tmppath.empty()){
                internalpath_ = tmppath;
            } else{
                internalpath_ = "";
            }
            /*
             * 注意底层并不判断共享文件夹是否已经存在
             * */
            ret = share->createSharefolder(vmname_,
                                           foldername_,
                                           serverpath_,
                                           write_,
                                           automount_,
                                           internalpath_,
                                           callback_);
            std::cout << "调用createSharefolder方法" << endl;
            response->set_vcamethod("createSharefolder");
            response->set_code(ret);
            response->set_data(callback_);

        }
            break;

        case removeSharedFolder: {
            string callback_;

            ret = share->removeSharedFolder(vmname_,
                                            foldername_,
                                            callback_);
            std::cout << "调用removeSharedFolder方法" << endl;
            response->set_vcamethod("removeSharedFolder");
            response->set_code(ret);
            response->set_data(callback_);
        }
            break;

    }

    return grpc::Status::OK;
}

grpc::Status vca_controller::VmConfig(grpc::ServerContext *context, const Qigrpc::requestConfig *request,
                                      Qigrpc::vcaReply *response) {

    printf("%s %d\n", __func__, __LINE__);

    bool ret = false;
    int method_id = request->vcamethod();
    const string &vmname_ = request->vmname();
    auto *config = new vcaVmConfigure();

    switch (method_id) {
        case resetCpuCount: {
            string callback_;
            int cpucount_ = request->cpucount();
            ret = config->resetCpuCount(vmname_,/*虚拟机名字*/
                                        cpucount_,/*cpu数量*/
                                        callback_);
            std::cout << "调用resetCpuCount方法" << endl;
            response->set_vcamethod("resetCpuCount");
            response->set_code(ret);
            response->set_data(callback_);
        }
            break;

        case resetMemSize: {
            string callback_;
            int memorysize_ = request->memorysize();
            ret = config->resetMemSize(vmname_,/*虚拟机名字*/
                                       memorysize_,/*内存大小*/
                                       callback_);
            std::cout << "调用resetMemSize方法" << endl;
            response->set_vcamethod("resetMemSize");
            response->set_code(ret);
            response->set_data(callback_);
        }
            break;

        case resetGraphics: {
            string callback_;
            int ramsize_ = request->ramsize();
            int monitorcount_ = request->monitorcount();
            const string &cardcontroller = "VBoxSVGA";
            bool enable3d_ = request->enable3d();

            ret = config->resetGraphics(vmname_,/*虚拟机名字*/
                                        ramsize_,/*显存大小*/
                                        monitorcount_,/*屏幕数量*/
                                        cardcontroller,/*显卡控制器*/
                                        enable3d_,/*是否打开3D*/
                                        callback_);
            std::cout << "调用resetGraphics方法" << endl;
            response->set_vcamethod("resetGraphics");
            response->set_code(ret);
            response->set_data(callback_);
        }
            break;
    }

    return grpc::Status::OK;
}

grpc::Status vca_controller::VmUSB(grpc::ServerContext *context, const Qigrpc::requestUSB *request,
                                   Qigrpc::vcaReply *response) {

    printf("%s %d\n", __func__, __LINE__);

    bool ret = false;
    int method_id = request->vcamethod();
    const string &vmname_ = request->vmname();
    const string &usbctlname_ = request->usbname();

    auto *usb = new vcaVmConfigure();

    switch (method_id) {
        case addUSBController: {
            string callback_;
            const string &usbtype_ = request->usbtype();

            ret = usb->addUSBController(vmname_,
                                        usbctlname_,
                                        usbtype_,
                                        callback_);
            std::cout << "调用addUSBController方法" << endl;
            response->set_vcamethod("addUSBController");
            response->set_code(ret);
            response->set_data(callback_);
        }
            break;

        case delUSBController: {
            string callback_;

            ret = usb->delUSBController(vmname_,
                                        usbctlname_,
                                        callback_);
            std::cout << "调用delUSBController方法" << endl;
            response->set_vcamethod("delUSBController");
            response->set_code(ret);
            response->set_data(callback_);
        }
            break;

    }

    return grpc::Status::OK;
}

grpc::Status vca_controller::VmAudio(grpc::ServerContext *context, const Qigrpc::requestAudio *request,
                                     Qigrpc::vcaReply *response) {

    printf("%s %d\n", __func__, __LINE__);

    bool ret = false;
    int method_id = request->vcamethod();
    const string &vmname_ = request->vmname();
    auto *audio = new vcaVmConfigure();

    switch (method_id) {
        case setAudio: {
            string callback_;
            bool in_ = request->audioin();
            bool out_ = request->audioout();

            ret = audio->setAudio(vmname_,
                                  in_,
                                  out_,
                                  callback_);
            std::cout << "调用setAudio方法" << endl;
            response->set_vcamethod("setAudio");
            response->set_code(ret);
            response->set_data(callback_);
        }
            break;

        case switchAudio: {
            string callback_;
            bool enable_ = request->audioswitch();
            ret = audio->switchAudio(vmname_,
                                     enable_,
                                     callback_);
            std::cout << "调用switchAudio方法" << endl;
            response->set_vcamethod("switchAudio");
            response->set_code(ret);
            response->set_data(callback_);
        }
            break;

    }

    return grpc::Status::OK;
}

grpc::Status vca_controller::VmSerial(grpc::ServerContext *context, const Qigrpc::requestSerial *request,
                                      Qigrpc::vcaReply *response) {


    printf("%s %d\n", __func__, __LINE__);

    bool ret = false;
    int method_id = request->vcamethod();
    const string &vmname_ = request->vmname();
    int slot_ = request->slot();
    auto *slots = new vcaVmConfigure();

    switch (method_id) {
        case addSerialPort: {
            string callback_;

            ret = slots->addSerialPort(vmname_,
                                       slot_,
                                       callback_);
            std::cout << "调用addSerialPort方法" << endl;
            response->set_vcamethod("addSerialPort");
            response->set_code(ret);
            response->set_data(callback_);
        }
            break;

        case delSerialPort: {
            string callback_;

            ret = slots->delSerialPort(vmname_,
                                       slot_,
                                       callback_);
            std::cout << "调用delSerialPort方法" << endl;
            response->set_vcamethod("delSerialPort");
            response->set_code(ret);
            response->set_data(callback_);
        }
            break;

    }

    return grpc::Status::OK;
}

grpc::Status vca_controller::P2P(grpc::ServerContext *context, const Qigrpc::requestP2P *request,
                                 Qigrpc::vcaReply *response) {
    printf("%s %d\n", __func__, __LINE__);

    bool ret = false;
    int method_id = request->vcamethod();
    auto *p2p = new vca_murderHook();

    switch (method_id) {
        case startTracker: {
            ret = p2p->startTracker();
            std::cout << "调用startTracker方法" << endl;
            response->set_vcamethod("startTracker");
            response->set_code(ret);
            response->set_data("");
        }
            break;

        case stopTracker: {
            ret = p2p->stopTracker();
            std::cout << "调用stopTracker方法" << endl;
            response->set_vcamethod("stopTracker");
            response->set_code(ret);
            response->set_data("");
        }
            break;

        case produceSeed: {
            const string &sourceFilePath_ = request->sharefilepath();
            const string &sourceSeedPath_ = request->shareseedpath();
            const string &localHost_ = request->shareip();
            const string &targetHost_ = request->downloadip();
            const string &targetSeedPath_ = request->downloadseedpath();

            ret = p2p->produceSeed(sourceFilePath_,
                                   sourceSeedPath_,
                                   localHost_,
                                   targetHost_,
                                   targetSeedPath_);

            std::cout << "调用produceSeed方法" << endl;
            response->set_vcamethod("produceSeed");
            response->set_code(ret);
            response->set_data("");
        }
            break;

        case startShare: {
            const string &sourceFilePath_ = request->sharefilepath();
            const string &sourceSeedPath_ = request->shareseedpath();
            const string &localHost_ = request->shareip();
            ret = p2p->startShare(sourceFilePath_,
                                  sourceSeedPath_,
                                  localHost_);

            std::cout << "调用startShare方法" << endl;
            response->set_vcamethod("startShare");
            response->set_code(ret);
            response->set_data("");
        }
            break;

        case downloadFile: {
            const string &downloadPath_ = request->downloadfilepath();
            const string &localSeedPath_ = request->downloadseedpath();
            const string &localHost_ = request->downloadip();
            ret = p2p->downloadFile(downloadPath_,
                                    localSeedPath_,
                                    localHost_);

            std::cout << "调用downloadFile方法" << endl;
            response->set_vcamethod("downloadFile");
            response->set_code(ret);
            response->set_data("");

        }
            break;

        case stopShare: {
            ret = p2p->stopShare();
            std::cout << "调用stopShare方法" << endl;
            response->set_vcamethod("stopShare");
            response->set_code(ret);
            response->set_data("");
        }
            break;
    }

    return grpc::Status::OK;
}

grpc::Status vca_controller::VmExtends(grpc::ServerContext *context, const Qigrpc::requestVmExtends *request,
                                       Qigrpc::vcaReply *response) {

    printf("%s %d\n", __func__, __LINE__);

    bool ret = false;
    int method_id = request->vcamethod();

    auto *vca = new QiVCA();

    switch (method_id) {
        case makeFloatDisk: {
            string callback_;
            long int size_ = request->size();
            const string &disknameorid_ = request->diskname();
            const string &diskformat_ = request->diskformattype();
            const string &basefolder_ = request->basefolder();

            ret = vca->makeFloatDisk(disknameorid_,
                                     diskformat_,/*磁盘类型*/
                                     basefolder_,/*磁盘存储目录*/
                                     size_,
                                     callback_);
            response->set_code(ret);
            response->set_data(callback_);
            response->set_vcamethod("makeFloatDisk");
        }
            break;

        case addDiskToVm: {
            string callback_;
            const string &vmname_ = request->vmname();
            const string &diskname_ = request->diskname();
            const string &diskformat_ = request->diskformattype();
            const string &basefolder_ = request->basefolder();
            const string &storagecontroller_ = request->storagecontroller();
            int slot_ = request->slot();
            ret = vca->addDiskToVm(vmname_,
                                   diskname_,
                                   diskformat_,/*磁盘类型 VDI VHD VDMK*/
                                   storagecontroller_,/*磁盘控制器类型:IDE;SATA;SCSI;Floppy;SAS;USB;PCIe*/
                                   basefolder_, /*磁盘存储目录*/
                                   slot_, /*1, 2, 3支持3个磁盘槽位置*/
                                   callback_);
            response->set_vcamethod("addDiskToVm");
            response->set_data(callback_);
            response->set_code(ret);

        }
            break;

        case detachDiskFromVm: {
            string callback_;
            const string &vmname_ = request->vmname();
            int slot_ = request->slot();
            ret = vca->detachDiskFromVm(vmname_,
                                        slot_,
                                        callback_);

            response->set_vcamethod("detachDiskFromVm");
            response->set_data(callback_);
            response->set_code(ret);
        }
            break;

        case destroyFloatDiskWithoutAttach: {
            string callback_;
            const string &diskuuid_ = request->diskuuid();
            ret = vca->destroyFloatDiskWithoutAttach(diskuuid_,
                                                     callback_);

            response->set_vcamethod("destroyFloatDiskWithoutAttach");
            response->set_data(callback_);
            response->set_code(ret);
        }
            break;

    }

    return grpc::Status::OK;
}