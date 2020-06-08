//
// Created by root on 2019/12/3.
//

#ifndef QIGRPC_VCA_HANDLETYPE_H
#define QIGRPC_VCA_HANDLETYPE_H

#include "qigrpc.grpc.pb.h"
#include <grpcpp/grpcpp.h>


struct HandlerContextBase {
    // 当前对象类型，用于确定是当前请求的服务类型
    int type_;
    // 当前处理状态（处理分为两步：1处理请求构建响应数据；2发送响应）
    // 这里记录一下完成到哪一步了，以便进行相关操作
    int status_; // (1构建响应完成；2发送完成)
    // rpc的上下文，允许通过它进行诸如压缩、身份验证，以及把元数据发回客户端等。
    grpc::ServerContext ctx_;
};

template<typename RequestType, typename ReplyType>
struct HandlerContext : public HandlerContextBase {
    // 用于接收客户端发送的请求
    RequestType req_;
    // 用于发送响应给客户端
    ReplyType rep_;
    // 发送到客户端的方法对象
    grpc::ServerAsyncResponseWriter<ReplyType> responder_;

    //================================================
    // 构造函数
    HandlerContext()
            : responder_(&ctx_) {}

};

/*处理请求测试请求*/
typedef HandlerContext<Qigrpc::requestNull, Qigrpc::vcaReply> HandlerTestContext;

/*处理请求主机信息请求*/
typedef HandlerContext<Qigrpc::requestNull, Qigrpc::vcaReply> HandlerHostContext;

/*处理虚拟机通用操作请求*/
typedef HandlerContext<Qigrpc::requestSimple, Qigrpc::vcaReply> HandlerVmCommonContext;

/*处理虚拟机装机请求*/
typedef HandlerContext<Qigrpc::requestInstall, Qigrpc::vcaReply> HandlerVmInstallContext;

/*处理虚拟机快照请求*/
typedef HandlerContext<Qigrpc::requestSnapshot, Qigrpc::vcaReply> HandlerVmSnapshotContext;

/*处理虚拟机克隆请求*/
typedef HandlerContext<Qigrpc::requestClone, Qigrpc::vcaReply> HandlerVmCloneContext;

/*处理虚拟机的导入和导出请求*/
typedef HandlerContext<Qigrpc::requestImpAndexportVM, Qigrpc::vcaReply> HandlerVmImpExpContext;

/*处理虚拟机网卡请求*/
typedef HandlerContext<Qigrpc::requestNetwork, Qigrpc::vcaReply> HandlerVmNetworkContext;

/*处理虚拟机的rdp请求*/
typedef HandlerContext<Qigrpc::requestRdp, Qigrpc::vcaReply> HandlerVmRdpContext;

/*处理虚拟机的屏幕录制请求*/
typedef HandlerContext<Qigrpc::requestRecord, Qigrpc::vcaReply> HandlerVmRecordContext;

/*处理虚拟机共享文件夹*/
typedef HandlerContext<Qigrpc::requestSharefolder, Qigrpc::vcaReply> HandlerVmSharefolderContext;

/*处理虚拟机的配置请求*/
typedef HandlerContext<Qigrpc::requestConfig, Qigrpc::vcaReply> HandlerVmConfigContext;

/*处理虚拟机usb*/
typedef HandlerContext<Qigrpc::requestUSB, Qigrpc::vcaReply> HandlerVmUSBContext;

/*处理虚拟机声卡*/
typedef HandlerContext<Qigrpc::requestAudio, Qigrpc::vcaReply> HandlerVmAudioContext;

/*处理虚拟机串口*/
typedef HandlerContext<Qigrpc::requestSerial, Qigrpc::vcaReply> HandlerVmSerialContext;

/*p2p*/
typedef HandlerContext<Qigrpc::requestP2P, Qigrpc::vcaReply> HandlerP2PContext;

/*requestVmExtends*/
typedef HandlerContext<Qigrpc::requestVmExtends, Qigrpc::vcaReply> HandlerVmExtendsContext;



#endif //QIGRPC_VCA_HANDLETYPE_H
