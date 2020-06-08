//
// Created by root on 2019/12/3.
//

#ifndef QIGRPC_VCA_CONTROLLER_H
#define QIGRPC_VCA_CONTROLLER_H

#include "qigrpc.grpc.pb.h"
#include <grpcpp/grpcpp.h>
#include <iostream>
#include <backward/strstream>
#include "vca_handleType.h"
#include "vca_threadpool.h"
#include "../monitor/vca_monitorHook.h"
#include "../include/vca_core.h"
#include "../include/vca_vmConfigure.h"
#include "../monitor/vca_monitorHook.h"
#include "../murder/vca_murderHook.h"

using namespace std;


class vca_controller {
public:
    /*************************************************/

    static unsigned long get_tid();

    /*
     * 测试接口
     * */
    static grpc::Status Test(grpc::ServerContext *context,
                             const Qigrpc::requestNull *request,
                             Qigrpc::vcaReply *response);

    /*
     * 虚拟机普通操作服务:
     *     startUp = 0,    //开机
     *     shutDown = 1,   //关机
     *     powerOff = 2,   //断电
     *     pauses = 3, //挂起
     *     resume = 4, //恢复
     *     vmSleep = 5,    //挂起到磁盘
     *     restart = 6,    //重启
     *     getVmInfo = 7,  //获取虚拟机信息
     *     listVMs = 8,    //获取虚拟机列表
     *     deleteVM = 9,   //删除虚拟机
     * */
    static grpc::Status VmCommon(grpc::ServerContext *context,
                                 const Qigrpc::requestSimple *request,
                                 Qigrpc::vcaReply *response);

    /*
     * 获取主机信息服务
     * */
    static grpc::Status HostInfo(grpc::ServerContext *context,
                                 const Qigrpc::requestNull *request,
                                 Qigrpc::vcaReply *response);

    /*
     * 虚拟机装机服务
     * */
    static grpc::Status VmInstall(grpc::ServerContext *context,
                                  const Qigrpc::requestInstall *request,
                                  Qigrpc::vcaReply *response);

    /*
     *虚拟机快照服务
     * */
    static grpc::Status VmOperateSnapshot(grpc::ServerContext *context,
                                          const Qigrpc::requestSnapshot *request,
                                          Qigrpc::vcaReply *response);

    /*
     * 虚拟机克隆服务
     * */
    static grpc::Status VmClone(grpc::ServerContext *context,
                                const Qigrpc::requestClone *request,
                                Qigrpc::vcaReply *response);

    /*
     *虚拟机导入导出服务
     * */
    static grpc::Status VmImpAndExp(grpc::ServerContext *context,
                                    const Qigrpc::requestImpAndexportVM *request,
                                    Qigrpc::vcaReply *response);

    /*
     *虚拟机网络设置服务
     * */
    static grpc::Status VmNetwork(grpc::ServerContext *context,
                                  const Qigrpc::requestNetwork *request,
                                  Qigrpc::vcaReply *response);

    /*
     * 虚拟机rdp服务
     * */
    static grpc::Status VmRdp(grpc::ServerContext *context,
                              const Qigrpc::requestRdp *request,
                              Qigrpc::vcaReply *response);

    /*
     *虚拟机录屏服务
     * */
    static grpc::Status VmRecord(grpc::ServerContext *context,
                                 const Qigrpc::requestRecord *request,
                                 Qigrpc::vcaReply *response);

    /*
     *虚拟机共享文件夹服务
     * */
    static grpc::Status VmSharefolder(grpc::ServerContext *context,
                                      const Qigrpc::requestSharefolder *request,
                                      Qigrpc::vcaReply *response);

    /*
     *虚拟机配置服务
     * */
    static grpc::Status VmConfig(grpc::ServerContext *context,
                                 const Qigrpc::requestConfig *request,
                                 Qigrpc::vcaReply *response);

    /*
     *虚拟机USB服务
     * */
    static grpc::Status VmUSB(grpc::ServerContext *context,
                              const Qigrpc::requestUSB *request,
                              Qigrpc::vcaReply *response);

    /*
     *虚拟机声卡服务
     * */
    static grpc::Status VmAudio(grpc::ServerContext *context,
                                const Qigrpc::requestAudio *request,
                                Qigrpc::vcaReply *response);

    /*
     * 虚拟机串口服务
     * */
    static grpc::Status VmSerial(grpc::ServerContext *context,
                                 const Qigrpc::requestSerial *request,
                                 Qigrpc::vcaReply *response);

    /*
     * p2p
     * */
    static grpc::Status P2P(grpc::ServerContext *context,
                                 const Qigrpc::requestP2P *request,
                                 Qigrpc::vcaReply *response);

    /*
     * requestVmExtends
     * */
    static grpc::Status VmExtends(grpc::ServerContext *context,
                            const Qigrpc::requestVmExtends *request,
                            Qigrpc::vcaReply *response);


};

#endif //QIGRPC_VCA_CONTROLLER_H
