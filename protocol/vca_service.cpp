// > g++ -o aservice2 async_service2.cpp  Qigrpc.grpc.pb.cc Qigrpc.pb.cc -std=c++11 -I. -lgrpc++ -lgrpc -lprotobuf -lgpr -lz -lcares -laddress_sorting -lpthread -Wno-deprecated

#include "vca_threadpool.h"
#include "qigrpc.grpc.pb.h"
#include <grpcpp/grpcpp.h>
#include "vca_handleType.h"
#include "vca_controller.h"
#include "vca_service.h"
#include <memory>
#include <iostream>

using namespace std;

vcaService::vcaService() {
    // 先创建三个类型接口的请求处理上下文对象
    Test = new HandlerTestContext;
    Test->status_ = 1; // 设置状态为1（因为只需要区分是否已经发送响应完成）
    Test->type_ = 1; // 设置类型为1

    Host = new HandlerHostContext;
    Host->status_ = 1;
    Host->type_ = 2;

    VmCommon = new HandlerVmCommonContext;
    VmCommon->status_ = 1;
    VmCommon->type_ = 3;

    VmInstall = new HandlerVmInstallContext;
    VmInstall->status_ = 1;
    VmInstall->type_ = 4;

    VmSnapshot = new HandlerVmSnapshotContext;
    VmSnapshot->status_ = 1;
    VmSnapshot->type_ = 5;

    VmClone = new HandlerVmCloneContext;
    VmClone->status_ = 1;
    VmClone->type_ = 6;

    VmImpExp = new HandlerVmImpExpContext;
    VmImpExp->status_ = 1;
    VmImpExp->type_ = 7;

    VmNetwork = new HandlerVmNetworkContext;
    VmNetwork->status_ = 1;
    VmNetwork->type_ = 8;

    VmRdp = new HandlerVmRdpContext;
    VmRdp->status_ = 1;
    VmRdp->type_ = 9;

    VmRecord = new HandlerVmRecordContext;
    VmRecord->status_ = 1;
    VmRecord->type_ = 10;

    VmSharefolder = new HandlerVmSharefolderContext;
    VmSharefolder->status_ = 1;
    VmSharefolder->type_ = 11;

    VmConfig = new HandlerVmConfigContext;
    VmConfig->status_ = 1;
    VmConfig->type_ = 12;

    VmUSB = new HandlerVmUSBContext;
    VmUSB->status_ = 1;
    VmUSB->type_ = 13;

    VmAudio = new HandlerVmAudioContext;
    VmAudio->status_ = 1;
    VmAudio->type_ = 14;

    VmSerial = new HandlerVmSerialContext;
    VmSerial->status_ = 1;
    VmSerial->type_ = 15;

    /*P2P*/
    P2P = new HandlerP2PContext;
    P2P->status_ = 1;
    P2P->type_ = 16;

    /*VmExtends*/
    extend = new HandlerVmExtendsContext;
    extend->status_ = 1;
    extend->type_ = 17;
}

vcaService::~vcaService() = default;


void vcaService::RegisterRequest(Qigrpc::Server::AsyncService &asyncService,
                                 std::unique_ptr<grpc::ServerCompletionQueue> &cqPtr) {

    // 注册测试服务到异步服务器
    asyncService.RequestTest(&Test->ctx_         /*服务上下文对象*/,
                             &Test->req_         /*用于接收请求的对象*/,
                             &Test->responder_   /*异步写响应对象*/,
                             cqPtr.get()        /*新的调用使用的完成队列*/,
                             cqPtr.get()        /*通知使用的完成队列*/,
                             Test                /*唯一标识tag*/);

    // 注册获取主机信息服务到异步服务器
    asyncService.RequestHostInfo(&Host->ctx_,
                                 &Host->req_,
                                 &Host->responder_,
                                 cqPtr.get(),
                                 cqPtr.get(),
                                 Host);

    // 注册虚拟机操作服务到异步服务器
    asyncService.RequestVmCommon(&VmCommon->ctx_,
                                 &VmCommon->req_,
                                 &VmCommon->responder_,
                                 cqPtr.get(),
                                 cqPtr.get(),
                                 VmCommon);

    // 注册虚拟机装机服务到异步服务器
    asyncService.RequestVmInstall(&VmInstall->ctx_,
                                  &VmInstall->req_,
                                  &VmInstall->responder_,
                                  cqPtr.get(),
                                  cqPtr.get(),
                                  VmInstall);

    // 注册虚拟机快照服务到异步服务器
    asyncService.RequestVmOperateSnapshot(&VmSnapshot->ctx_,
                                          &VmSnapshot->req_,
                                          &VmSnapshot->responder_,
                                          cqPtr.get(),
                                          cqPtr.get(),
                                          VmSnapshot);

    // 注册虚拟机克隆服务到异步服务器
    asyncService.RequestVmClone(&VmClone->ctx_,
                                &VmClone->req_,
                                &VmClone->responder_,
                                cqPtr.get(),
                                cqPtr.get(),
                                VmClone);

    // 注册虚拟机导入导出服务到异步服务器
    asyncService.RequestVmImpAndExp(&VmImpExp->ctx_,
                                    &VmImpExp->req_,
                                    &VmImpExp->responder_,
                                    cqPtr.get(),
                                    cqPtr.get(),
                                    VmImpExp);


    // 注册虚拟机网卡服务到异步服务器
    asyncService.RequestVmNetwork(&VmNetwork->ctx_,
                                  &VmNetwork->req_,
                                  &VmNetwork->responder_,
                                  cqPtr.get(),
                                  cqPtr.get(),
                                  VmNetwork);

    // 注册虚拟机rdp服务到异步服务器
    asyncService.RequestVmRdp(&VmRdp->ctx_,
                              &VmRdp->req_,
                              &VmRdp->responder_,
                              cqPtr.get(),
                              cqPtr.get(),
                              VmRdp);

    // 注册虚拟机录屏服务到异步服务器
    asyncService.RequestVmRecord(&VmRecord->ctx_,
                                 &VmRecord->req_,
                                 &VmRecord->responder_,
                                 cqPtr.get(),
                                 cqPtr.get(),
                                 VmRecord);

    // 注册虚拟机共享文件夹服务到异步服务器
    asyncService.RequestVmSharefolder(&VmSharefolder->ctx_,
                                      &VmSharefolder->req_,
                                      &VmSharefolder->responder_,
                                      cqPtr.get(),
                                      cqPtr.get(),
                                      VmSharefolder);

    // 注册虚拟机配置服务到异步服务器
    asyncService.RequestVmConfig(&VmConfig->ctx_,
                                 &VmConfig->req_,
                                 &VmConfig->responder_,
                                 cqPtr.get(),
                                 cqPtr.get(),
                                 VmConfig);

    // 注册虚拟机USB服务到异步服务器
    asyncService.RequestVmUSB(&VmUSB->ctx_,
                              &VmUSB->req_,
                              &VmUSB->responder_,
                              cqPtr.get(),
                              cqPtr.get(),
                              VmUSB);

    // 注册虚拟机声卡服务到异步服务器
    asyncService.RequestVmAudio(&VmAudio->ctx_,
                                &VmAudio->req_,
                                &VmAudio->responder_,
                                cqPtr.get(),
                                cqPtr.get(),
                                VmAudio);

    // 注册虚拟机串口服务到异步服务器
    asyncService.RequestVmSerial(&VmSerial->ctx_,
                                 &VmSerial->req_,
                                 &VmSerial->responder_,
                                 cqPtr.get(),
                                 cqPtr.get(),
                                 VmSerial);

    // 注册P2P服务到异步服务器
    asyncService.RequestP2P(&P2P->ctx_,
                            &P2P->req_,
                            &P2P->responder_,
                            cqPtr.get(),
                            cqPtr.get(),
                            P2P);

    // 注册VmExtends服务到异步服务器
    asyncService.RequestVmExtends(&extend->ctx_,
                                  &extend->req_,
                                  &extend->responder_,
                                  cqPtr.get(),
                                  cqPtr.get(),
                                  extend);
}


void vcaService::Handler(Qigrpc::Server::AsyncService &service, std::unique_ptr<grpc::ServerCompletionQueue> &cq_ptr) {

    /*
     * 创建线程池，使用4个工作线程，用于构建请求的响应
     */
    ThreadPool pool(17);

    /*
     * 异步服务这里不能使用 server->Wait() 来等待处理，因为是异步服务
     * 服务器会把到达的请求放入队列，需要自己从完成队列取出请求进行处理
     * 所以这里需要一个死循环来获取请求并进行处理
     * */

    while (true) {
        // 前面已经注册了请求处理，这里阻塞从完成队列中取出一个请求进行处理
        HandlerContextBase *htc = nullptr;
        bool ok = false;
        GPR_ASSERT(cq_ptr->Next((void **) &htc, &ok));
        GPR_ASSERT(ok);
        // 根据tag判断是哪一个请求
        // 因为前面注册请求处理的时候使用的就是对象地址
        // 所以这里直接从map里面取出来判断即可
        int i = htc->type_;
        // 判断状态，看是不是已经响应发送了
        if (htc->status_ == 2) {
            // 因为这里并不是多态类，必须根据类型操作
            switch (i) {
                case 1: {
                    // 释放对象（这里未对这个对象进行复用）
                    delete (HandlerTestContext *) htc;
                }
                    break;
                case 2: {
                    delete (HandlerHostContext *) htc;
                }
                    break;
                case 3: {
                    delete (HandlerVmCommonContext *) htc;
                }
                    break;
                case 4: {
                    delete (HandlerVmInstallContext *) htc;

                }
                    break;
                case 5: {
                    delete (HandlerVmSnapshotContext *) htc;

                }
                    break;
                case 6: {
                    delete (HandlerVmCloneContext *) htc;

                }
                    break;
                case 7: {
                    delete (HandlerVmImpExpContext *) htc;

                }
                    break;
                case 8: {
                    delete (HandlerVmNetworkContext *) htc;

                }
                    break;
                case 9: {
                    delete (HandlerVmRdpContext *) htc;

                }
                    break;
                case 10: {
                    delete (HandlerVmRecordContext *) htc;

                }
                    break;
                case 11: {
                    delete (HandlerVmSharefolderContext *) htc;

                }
                    break;
                case 12: {
                    delete (HandlerVmConfigContext *) htc;

                }
                    break;
                case 13: {
                    delete (HandlerVmUSBContext *) htc;

                }
                    break;
                case 14: {
                    delete (HandlerVmAudioContext *) htc;

                }
                    break;
                case 15: {
                    delete (HandlerVmSerialContext *) htc;

                }
                    break;

                    /*P2P*/
                case 16: {
                    delete (HandlerP2PContext *) htc;

                }
                    break;

                    /*VmExtends*/
                case 17: {
                    delete (HandlerVmExtendsContext *) htc;

                }
                    break;
            }
            continue; // 回到从完成队列获取下一个
        }

        // 重新创建一个请求处理上下文对象(以便能够接受下一个请求进行处理)
        switch (i) {
            case 1: {
                auto *Test_ = new HandlerTestContext;
                Test_->status_ = 1;    // 设置状态为1
                Test_->type_ = 1;    // 设置类型为1
                service.RequestTest(&Test_->ctx_, &Test_->req_, &Test_->responder_,
                                    cq_ptr.get(), cq_ptr.get(), Test_);
            }
                break;

            case 2: {
                auto *Host_ = new HandlerHostContext;
                Host_->status_ = 1;    // 设置状态为1
                Host_->type_ = 2;    // 设置类型为2
                service.RequestHostInfo(&Host_->ctx_, &Host_->req_, &Host_->responder_,
                                        cq_ptr.get(), cq_ptr.get(), Host_);
            }
                break;

            case 3: {
                auto *VmCommon_ = new HandlerVmCommonContext;
                VmCommon_->status_ = 1;    // 设置状态为1
                VmCommon_->type_ = 3;    // 设置类型为3
                service.RequestVmCommon(&VmCommon_->ctx_, &VmCommon_->req_, &VmCommon_->responder_,
                                        cq_ptr.get(), cq_ptr.get(), VmCommon_);
            }
                break;

            case 4: {
                auto *VmInstall_ = new HandlerVmInstallContext;
                VmInstall_->status_ = 1;    // 设置状态为1
                VmInstall_->type_ = 4;    // 设置类型为4
                service.RequestVmInstall(&VmInstall_->ctx_, &VmInstall_->req_, &VmInstall_->responder_,
                                         cq_ptr.get(), cq_ptr.get(), VmInstall_);
            }
                break;

            case 5: {
                auto *VmSnapshot_ = new HandlerVmSnapshotContext;
                VmSnapshot_->status_ = 1;    // 设置状态为1
                VmSnapshot_->type_ = 5;    // 设置类型为5
                service.RequestVmOperateSnapshot(&VmSnapshot_->ctx_, &VmSnapshot_->req_, &VmSnapshot_->responder_,
                                                 cq_ptr.get(), cq_ptr.get(), VmSnapshot_);
            }
                break;

            case 6: {
                auto *VmClone_ = new HandlerVmCloneContext;
                VmClone_->status_ = 1;    // 设置状态为1
                VmClone_->type_ = 6;    // 设置类型为6
                service.RequestVmClone(&VmClone_->ctx_, &VmClone_->req_, &VmClone_->responder_,
                                       cq_ptr.get(), cq_ptr.get(), VmClone_);
            }
                break;

            case 7: {
                auto *VmImpExp_ = new HandlerVmImpExpContext;
                VmImpExp_->status_ = 1;    // 设置状态为1
                VmImpExp_->type_ = 7;    // 设置类型为7
                service.RequestVmImpAndExp(&VmImpExp_->ctx_, &VmImpExp_->req_, &VmImpExp_->responder_,
                                           cq_ptr.get(), cq_ptr.get(), VmImpExp_);
            }
                break;

            case 8: {
                auto *VmNetwork_ = new HandlerVmNetworkContext;
                VmNetwork_->status_ = 1;    // 设置状态为1
                VmNetwork_->type_ = 8;    // 设置类型为8
                service.RequestVmNetwork(&VmNetwork_->ctx_, &VmNetwork_->req_, &VmNetwork_->responder_,
                                         cq_ptr.get(), cq_ptr.get(), VmNetwork_);
            }
                break;

            case 9: {
                auto *VmRdp_ = new HandlerVmRdpContext;
                VmRdp_->status_ = 1;    // 设置状态为1
                VmRdp_->type_ = 9;    // 设置类型为9
                service.RequestVmRdp(&VmRdp_->ctx_, &VmRdp_->req_, &VmRdp_->responder_,
                                     cq_ptr.get(), cq_ptr.get(), VmRdp_);
            }
                break;

            case 10: {
                auto *VmRecord_ = new HandlerVmRecordContext;
                VmRecord_->status_ = 1;    // 设置状态为1
                VmRecord_->type_ = 10;    // 设置类型为10
                service.RequestVmRecord(&VmRecord_->ctx_, &VmRecord_->req_, &VmRecord_->responder_,
                                        cq_ptr.get(), cq_ptr.get(), VmRecord_);
            }
                break;

            case 11: {
                auto *VmSharefolder_ = new HandlerVmSharefolderContext;
                VmSharefolder_->status_ = 1;    // 设置状态为1
                VmSharefolder_->type_ = 11;    // 设置类型为11
                service.RequestVmSharefolder(&VmSharefolder_->ctx_, &VmSharefolder_->req_, &VmSharefolder_->responder_,
                                             cq_ptr.get(), cq_ptr.get(), VmSharefolder_);
            }
                break;

            case 12: {
                auto *VmConfig_ = new HandlerVmConfigContext;
                VmConfig_->status_ = 1;    // 设置状态为1
                VmConfig_->type_ = 12;    // 设置类型为12
                service.RequestVmConfig(&VmConfig_->ctx_, &VmConfig_->req_, &VmConfig_->responder_,
                                        cq_ptr.get(), cq_ptr.get(), VmConfig_);
            }
                break;

            case 13: {
                auto *VmUSB_ = new HandlerVmUSBContext;
                VmUSB_->status_ = 1;    // 设置状态为1
                VmUSB_->type_ = 13;    // 设置类型为13
                service.RequestVmUSB(&VmUSB_->ctx_, &VmUSB_->req_, &VmUSB_->responder_,
                                     cq_ptr.get(), cq_ptr.get(), VmUSB_);
            }
                break;

            case 14: {
                auto *VmAudio_ = new HandlerVmAudioContext;
                VmAudio_->status_ = 1;    // 设置状态为1
                VmAudio_->type_ = 14;    // 设置类型为14
                service.RequestVmAudio(&VmAudio_->ctx_, &VmAudio_->req_, &VmAudio_->responder_,
                                       cq_ptr.get(), cq_ptr.get(), VmAudio_);
            }
                break;

            case 15: {
                auto *VmSerial_ = new HandlerVmSerialContext;
                VmSerial_->status_ = 1;    // 设置状态为1
                VmSerial_->type_ = 15;    // 设置类型为15
                service.RequestVmSerial(&VmSerial_->ctx_, &VmSerial_->req_, &VmSerial_->responder_,
                                        cq_ptr.get(), cq_ptr.get(), VmSerial_);
            }
                break;

                /*P2P*/
            case 16: {
                auto *P2P_ = new HandlerP2PContext;
                P2P_->status_ = 1;    // 设置状态为1
                P2P_->type_ = 16;    // 设置类型为15
                service.RequestP2P(&P2P_->ctx_, &P2P_->req_, &P2P_->responder_,
                                   cq_ptr.get(), cq_ptr.get(), P2P_);
            }
                break;

                /*VmExtends*/
            case 17: {
                auto *extend_ = new HandlerVmExtendsContext;
                extend_->status_ = 1;    // 设置状态为1
                extend_->type_ = 17;    // 设置类型为15
                service.RequestVmExtends(&extend_->ctx_, &extend_->req_, &extend_->responder_,
                                   cq_ptr.get(), cq_ptr.get(), extend_);
            }
                break;

        }

        pool.enqueue([i, htc]() {
            // 根据type进行相应的处理
            switch (i) {
                case 1: /*Test的处理*/
                {
                    auto *h = (HandlerTestContext *) htc;
                    grpc::Status status = vca_controller::Test(&h->ctx_, &h->req_, &h->rep_);
                    // 设置状态为发送响应
                    h->status_ = 2;
                    // 调用responder_进行响应发送（异步）
                    h->responder_.Finish(h->rep_/*发送的响应*/, status/*状态码*/, htc/*请求处理的唯一tag*/);
                }
                    break;

                case 2: /*Host的处理*/
                {
                    auto *h = (HandlerHostContext *) htc;
                    grpc::Status status = vca_controller::HostInfo(&h->ctx_, &h->req_, &h->rep_);
                    // 设置状态为发送响应
                    h->status_ = 2;
                    // 调用responder_进行响应发送（异步）
                    h->responder_.Finish(h->rep_/*发送的响应*/, status/*状态码*/, htc/*请求处理的唯一tag*/);
                }
                    break;

                case 3: /*VmCommon的处理*/
                {
                    auto *h = (HandlerVmCommonContext *) htc;

                    grpc::Status status = vca_controller::VmCommon(&h->ctx_, &h->req_, &h->rep_);
                    // 设置状态为发送响应
                    h->status_ = 2;
                    // 调用responder_进行响应发送（异步）
                    h->responder_.Finish(h->rep_/*发送的响应*/, status/*状态码*/, htc/*请求处理的唯一tag*/);
                }
                    break;

                case 4: /*VmInstall的处理*/
                {
                    auto *h = (HandlerVmInstallContext *) htc;

                    grpc::Status status = vca_controller::VmInstall(&h->ctx_, &h->req_, &h->rep_);
                    // 设置状态为发送响应
                    h->status_ = 2;
                    // 调用responder_进行响应发送（异步）
                    h->responder_.Finish(h->rep_/*发送的响应*/, status/*状态码*/, htc/*请求处理的唯一tag*/);
                }
                    break;

                case 5: /*VmOperateSnapshot的处理*/
                {
                    auto *h = (HandlerVmSnapshotContext *) htc;

                    grpc::Status status = vca_controller::VmOperateSnapshot(&h->ctx_, &h->req_, &h->rep_);
                    // 设置状态为发送响应
                    h->status_ = 2;
                    // 调用responder_进行响应发送（异步）
                    h->responder_.Finish(h->rep_/*发送的响应*/, status/*状态码*/, htc/*请求处理的唯一tag*/);
                }
                    break;

                case 6: /*VmClone的处理*/
                {
                    auto *h = (HandlerVmCloneContext *) htc;

                    grpc::Status status = vca_controller::VmClone(&h->ctx_, &h->req_, &h->rep_);
                    // 设置状态为发送响应
                    h->status_ = 2;
                    // 调用responder_进行响应发送（异步）
                    h->responder_.Finish(h->rep_/*发送的响应*/, status/*状态码*/, htc/*请求处理的唯一tag*/);
                }
                    break;

                case 7: /*VmImpAndExp的处理*/
                {
                    auto *h = (HandlerVmImpExpContext *) htc;

                    grpc::Status status = vca_controller::VmImpAndExp(&h->ctx_, &h->req_, &h->rep_);
                    // 设置状态为发送响应
                    h->status_ = 2;
                    // 调用responder_进行响应发送（异步）
                    h->responder_.Finish(h->rep_/*发送的响应*/, status/*状态码*/, htc/*请求处理的唯一tag*/);
                }
                    break;

                case 8: /*VmNetwork的处理*/
                {
                    auto *h = (HandlerVmNetworkContext *) htc;

                    grpc::Status status = vca_controller::VmNetwork(&h->ctx_, &h->req_, &h->rep_);
                    // 设置状态为发送响应
                    h->status_ = 2;
                    // 调用responder_进行响应发送（异步）
                    h->responder_.Finish(h->rep_/*发送的响应*/, status/*状态码*/, htc/*请求处理的唯一tag*/);
                }
                    break;

                case 9: /*VmRdp的处理*/
                {
                    auto *h = (HandlerVmRdpContext *) htc;

                    grpc::Status status = vca_controller::VmRdp(&h->ctx_, &h->req_, &h->rep_);
                    // 设置状态为发送响应
                    h->status_ = 2;
                    // 调用responder_进行响应发送（异步）
                    h->responder_.Finish(h->rep_/*发送的响应*/, status/*状态码*/, htc/*请求处理的唯一tag*/);
                }
                    break;

                case 10: /*VmRecord的处理*/
                {
                    auto *h = (HandlerVmRecordContext *) htc;

                    grpc::Status status = vca_controller::VmRecord(&h->ctx_, &h->req_, &h->rep_);
                    // 设置状态为发送响应
                    h->status_ = 2;
                    // 调用responder_进行响应发送（异步）
                    h->responder_.Finish(h->rep_/*发送的响应*/, status/*状态码*/, htc/*请求处理的唯一tag*/);
                }
                    break;

                case 11: /*VmSharefolder的处理*/
                {
                    auto *h = (HandlerVmSharefolderContext *) htc;

                    grpc::Status status = vca_controller::VmSharefolder(&h->ctx_, &h->req_, &h->rep_);
                    // 设置状态为发送响应
                    h->status_ = 2;
                    // 调用responder_进行响应发送（异步）
                    h->responder_.Finish(h->rep_/*发送的响应*/, status/*状态码*/, htc/*请求处理的唯一tag*/);
                }
                    break;

                case 12: /*VmConfig的处理*/
                {
                    auto *h = (HandlerVmConfigContext *) htc;

                    grpc::Status status = vca_controller::VmConfig(&h->ctx_, &h->req_, &h->rep_);
                    // 设置状态为发送响应
                    h->status_ = 2;
                    // 调用responder_进行响应发送（异步）
                    h->responder_.Finish(h->rep_/*发送的响应*/, status/*状态码*/, htc/*请求处理的唯一tag*/);
                }
                    break;

                case 13: /*VmUSB的处理*/
                {
                    auto *h = (HandlerVmUSBContext *) htc;

                    grpc::Status status = vca_controller::VmUSB(&h->ctx_, &h->req_, &h->rep_);
                    // 设置状态为发送响应
                    h->status_ = 2;
                    // 调用responder_进行响应发送（异步）
                    h->responder_.Finish(h->rep_/*发送的响应*/, status/*状态码*/, htc/*请求处理的唯一tag*/);
                }
                    break;

                case 14: /*VmAudio的处理*/
                {
                    auto *h = (HandlerVmAudioContext *) htc;

                    grpc::Status status = vca_controller::VmAudio(&h->ctx_, &h->req_, &h->rep_);
                    // 设置状态为发送响应
                    h->status_ = 2;
                    // 调用responder_进行响应发送（异步）
                    h->responder_.Finish(h->rep_/*发送的响应*/, status/*状态码*/, htc/*请求处理的唯一tag*/);
                }
                    break;

                case 15: /*VmSerial的处理*/
                {
                    auto *h = (HandlerVmSerialContext *) htc;

                    grpc::Status status = vca_controller::VmSerial(&h->ctx_, &h->req_, &h->rep_);
                    // 设置状态为发送响应
                    h->status_ = 2;
                    // 调用responder_进行响应发送（异步）
                    h->responder_.Finish(h->rep_/*发送的响应*/, status/*状态码*/, htc/*请求处理的唯一tag*/);
                }
                    break;

                    /*P2P*/
                case 16: /*P2P的处理*/
                {
                    auto *h = (HandlerP2PContext *) htc;

                    grpc::Status status = vca_controller::P2P(&h->ctx_, &h->req_, &h->rep_);
                    // 设置状态为发送响应
                    h->status_ = 2;
                    // 调用responder_进行响应发送（异步）
                    h->responder_.Finish(h->rep_/*发送的响应*/, status/*状态码*/, htc/*请求处理的唯一tag*/);
                }
                    break;

                    /*P2P*/
                case 17: /*P2P的处理*/
                {
                    auto *h = (HandlerVmExtendsContext *) htc;

                    grpc::Status status = vca_controller::VmExtends(&h->ctx_, &h->req_, &h->rep_);
                    // 设置状态为发送响应
                    h->status_ = 2;
                    // 调用responder_进行响应发送（异步）
                    h->responder_.Finish(h->rep_/*发送的响应*/, status/*状态码*/, htc/*请求处理的唯一tag*/);
                }
                    break;
            }
        });
    }
}

void vcaService::Service() {
    // 服务构建器，用于构建同步或者异步服务
    grpc::ServerBuilder builder;

    // 添加监听的地址和端口，后一个参数用于设置认证方式，这里选择不认证
    builder.AddListeningPort("0.0.0.0:33333", grpc::InsecureServerCredentials());

    // 注册服务
    builder.RegisterService(&service);

    // 完成队列，用于与 gRPC 运行时异步通信
    cq_ptr = builder.AddCompletionQueue();

    // 构建服务器
    std::unique_ptr<grpc::Server> server(builder.BuildAndStart());

    RegisterRequest(service, cq_ptr);
    std::cout << ">>> VirtualizationCoreAPI(version 1.0.0) service is Runing..." << std::endl;

    Handler(service, cq_ptr);

}

int main() {

    auto *vca = new vcaService();
    vca->Service();

    return 0;
}
