//
// Created by root on 2019/12/6.
//

#ifndef VIRTUALIZATIONCOREAPI_VCA_SERVICE_H
#define VIRTUALIZATIONCOREAPI_VCA_SERVICE_H

#include "vca_threadpool.h"
#include "qigrpc.grpc.pb.h"
#include <grpcpp/grpcpp.h>
#include "vca_handleType.h"
#include "vca_controller.h"

class vcaService {

public:

    vcaService();

    ~vcaService();

    void RegisterRequest(Qigrpc::Server::AsyncService &asyncService,
                         std::unique_ptr<grpc::ServerCompletionQueue> &cqPtr);

    static void Handler(Qigrpc::Server::AsyncService &service,
                 std::unique_ptr<grpc::ServerCompletionQueue> &cq_ptr);

    void Service();


private:
    std::unique_ptr<grpc::ServerCompletionQueue> cq_ptr;    //请求队列

    Qigrpc::Server::AsyncService service;   //异步服务对象

    HandlerTestContext *Test;

    HandlerHostContext *Host;

    HandlerVmCommonContext *VmCommon;

    HandlerVmInstallContext *VmInstall;

    HandlerVmSnapshotContext *VmSnapshot;

    HandlerVmCloneContext *VmClone;

    HandlerVmImpExpContext *VmImpExp;

    HandlerVmNetworkContext *VmNetwork;

    HandlerVmRdpContext *VmRdp;

    HandlerVmRecordContext *VmRecord;

    HandlerVmSharefolderContext *VmSharefolder;

    HandlerVmConfigContext *VmConfig;

    HandlerVmUSBContext *VmUSB;

    HandlerVmAudioContext *VmAudio;

    HandlerVmSerialContext *VmSerial;

    /*p2p*/
    HandlerP2PContext *P2P;

    /*VmExtends*/
    HandlerVmExtendsContext *extend;
};

#endif //VIRTUALIZATIONCOREAPI_VCA_SERVICE_H
