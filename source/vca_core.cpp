#include <iostream>

#include "../include/vca_core.h"
#include "../include/vca_toCode.h"
#include <list>
using namespace std;


QiVCA::QiVCA() {

    vboxClient = VBox::virtualBoxClient();
    vbox = vboxClient->virtualBox();
    machines = vbox->machines();

}

QiVCA::~QiVCA() {

    vbox->Release();
    vboxClient->Release();

}

/*
 * 列举虚拟机, 返回一个jsonarray字符串
 * */
bool QiVCA::listVMs(string &vms, string &callbackdata) {
    Json::Value jsonarray;

    map<const string, const int> map;   //声明一个map,用于存储虚拟机{name:state}列表
    std::vector<VBox::MachineState, std::allocator<VBox::MachineState >> states;
    states = vbox->getMachineStates(machines);

    string vmdata;
    vca_toCode code;   //转码对象
    std::vector<string> vm_vector;  //声明一个容器,用于存储虚拟机名字
    std::vector<string>::iterator vm_iter;

    for (auto &machine : machines) {
        //将虚拟机名字存储容器
        vm_vector.push_back(code.to_utf8(VBox::utf16ToWide(machine->name())));
    }

    vm_iter = vm_vector.begin();
    while (vm_iter != vm_vector.end()) {
        // 将容器内的虚拟机取出，并检索其状态，存储到map
        vmdata = *vm_iter;
//        std::cout<<"vmname:"<<vmdata.data()<<endl;
//        std::cout<<"state:"<<static_cast<int>(vbox->findMachine(code.to_utf16(vmdata))->state())<<endl;
        if (!vmdata.empty()) {
            map.insert(
                    pair<const string, const int>(vmdata,
                                                  static_cast<int>(vbox->findMachine(code.to_utf16(vmdata))->state())));
        }

        vm_iter++;
    }

    if (map.empty()) {
        //如果map为空
        callbackdata = "Virtual machine does not exist!";
        return false;
    }
    //构造json数组
    jsonarray.append(code.mapTojsonstr(map)).toStyledString();
    //赋值
    vms = writer.write(jsonarray);

    return true;
}

/*
 * 启动一台虚拟机，传入参数:虚拟机名字， ui类型
 * */
void QiVCA::startUp(const string &_vmname,
                    const string &ui,
                    int &data,
                    string &message) {

    try {

        vca_toCode code;   // string -> u6string
        std::u16string name = code.to_utf16(_vmname);
        std::u16string ui_type = code.to_utf16(ui);

        VBox::COMPtr<VBox::IMachine> machine;
        VBox::COMPtr<VBox::ISession> session;
        VBox::COMPtr<VBox::IProgress> progress;

        machine = vbox->findMachine(name);

        if (machine) {
            /*
              * 开机时候注意虚拟机状态，以下状态下不可进行开机操作:
              * 1.开机中，运行中，关机中
              * 2.保存状态中，恢复运行中
              * 3.快照中，删除快照中，恢复快照中
              * */
            switch (machine->state()) {
                case VBox::MachineState::PoweredOff:
                case VBox::MachineState::Aborted:
                case VBox::MachineState::Saved: {
                    session = vboxClient->session();
                    /*
                     * ui_type:gui or headless
                     * */
                    progress = machine->launchVMProcess(session, ui_type, u"");
                    progress->waitForCompletion(-1);
                    session->unlockMachine();
                    machine->Release();
                    data = 1;
                }
                    break;

                default:
                    data = 0;
                    message = "Virtual machine state is not poweroff or saved or aborted!";
                    machine->Release();
            }

#if 0
            auto evSource = vbox->eventSource();
            auto listener = evSource->createListener();
            evSource->registerListener(listener, {VBox::VBoxEventType::Any}, false);

            std::thread event_thread([evSource, listener]() {
                for (;;) {
                    auto event = evSource->getEvent(listener, 500);
                    if (!event)
                        continue;

                    if (auto stateChangedEvent = event->QueryInterface<VBox::IMachineStateChangedEvent>()) {

                        if (static_cast<int>(stateChangedEvent->state()) == 5) {
                            break;
                        }
                    }
                    evSource->eventProcessed(listener, event);
                }
            });

            event_thread.join();

#endif
        } else {
            machine->Release();
            data = 0;
            message = "Virtual machine does not exist!";
            return;
        }
        return;

    } catch (const VBox::COMError &err) {
        auto errorInfo = VBox::currentError();
        auto vboxError = errorInfo->QueryInterface<VBox::IVirtualBoxErrorInfo>();
        if (vboxError) {
            std::cout << "err :" << err.what();
            while (vboxError) {
                auto meg = vboxError->text();
                for (auto ch : meg) {
                    printf("%c", ch);
                }
                vboxError = vboxError->next();
            }
        }
    }
}

/*
 * 虚拟机关机, 传入参数:虚拟机名字
 * */
bool QiVCA::shutDown(const string &_vmname, string &callbackdata) {

    vca_toCode code;
    std::u16string name = code.to_utf16(_vmname);

    VBox::COMPtr<VBox::ISession> session;
    VBox::COMPtr<VBox::IMachine> machine;

    machine = vbox->findMachine(name);
    if (machine) {

        switch (machine->state()) {
            case VBox::MachineState::Running: {
                /*code*/
                session = vboxClient->session();
                machine->lockMachine(session, VBox::LockType::Shared);

                session->console()->powerButton();  //执行关机
                session->unlockMachine();
                machine->Release();
            }
                break;

            default:
                machine->Release();
                callbackdata = "Virtual machine state is not running!";
                return false;
        }

    } else {
        callbackdata = "Virtual machine does not exist!";
        machine->Release();
        return false;
    }

    return true;
}

/*
 * 虚拟机断电, 传入参数:虚拟机名字
 * */
bool QiVCA::powerOff(const string &_vmname, string &callbackdata) {

    vca_toCode code;
    std::u16string name = code.to_utf16(_vmname);

    VBox::COMPtr<VBox::ISession> session;
    VBox::COMPtr<VBox::IMachine> machine;

    machine = vbox->findMachine(name);
    if (machine) {
        switch (machine->state()) {
            case VBox::MachineState::Saving:
            case VBox::MachineState::Restoring:
            case VBox::MachineState::Saved:
            case VBox::MachineState::Aborted:
            case VBox::MachineState::PoweredOff:
            case VBox::MachineState::Snapshotting:
            case VBox::MachineState::RestoringSnapshot:
            case VBox::MachineState::DeletingSnapshot: {
                /*code*/
                callbackdata = "Virtual machine state is saved of poweroff!";
                machine->Release();
            }
                break;

            default:
                session = vboxClient->session();
                machine->lockMachine(session, VBox::LockType::Shared);

                session->console()->powerDown();
                session->unlockMachine();
                machine->Release();
                return true;
        }

    } else {
        callbackdata = "Virtual machine does not exist!";
        machine->Release();
        return false;
    }
}

/*
 *虚拟机挂起，传入参数:虚拟机名字
 * */
bool QiVCA::pause(const string &_vmname, string &callbackdata) {

    vca_toCode code;
    std::u16string name = code.to_utf16(_vmname);

    VBox::COMPtr<VBox::ISession> session;
    VBox::COMPtr<VBox::IMachine> machine;

    machine = vbox->findMachine(name);
    if (machine) {
        switch (machine->state()) {
            case VBox::MachineState::Running:
            case VBox::MachineState::Starting:
            case VBox::MachineState::Stopping: {
                /*code*/
                session = vboxClient->session();
                machine->lockMachine(session, VBox::LockType::Shared);
                session->console()->pause();
                session->unlockMachine();
                machine->Release();
            }
                break;

            default:
                machine->Release();
                callbackdata = "Virtual machine state is not running or starting or stoping!";
                return false;
        }
    } else {
        callbackdata = "Virtual machine does not exist!";

        machine->Release();
        return false;
    }

    return true;
}

/*
 *虚拟机由挂起->恢复执行,传参虚拟机名字
 * */
bool QiVCA::resume(const string &_vmname, string &callbackdata) {

    vca_toCode code;
    std::u16string name = code.to_utf16(_vmname);

    VBox::COMPtr<VBox::ISession> session;
    VBox::COMPtr<VBox::IMachine> machine;

    machine = vbox->findMachine(name);
    if (machine) {
        /*
         *虚拟机恢复运行时候需要注意虚拟机状态：
         * 1.虚拟机状态必须是paused
         * */
        switch (machine->state()) {
            case VBox::MachineState::Paused: {
                /*code*/
                session = vboxClient->session();
                machine->lockMachine(session, VBox::LockType::Shared);

                session->console()->resume();
                session->unlockMachine();
                machine->Release();
            }
                break;

            default:
                machine->Release();
                callbackdata = "Virtual machine state is not Paused!";
                return false;
        }

    } else {
        callbackdata = "Virtual machine does not exist!";

        machine->Release();
        return false;
    }

    return true;
}

/*
 * 虚拟机休眠，传入参数：虚拟机名字
 * */
bool QiVCA::vmSleep(const string &_vmname, string &callbackdata) {

    vca_toCode code;
    std::u16string name = code.to_utf16(_vmname);

    VBox::COMPtr<VBox::ISession> session;
    VBox::COMPtr<VBox::IMachine> machine;
    VBox::COMPtr<VBox::IMachine> mutablemachine;
    VBox::COMPtr<VBox::IProgress> progress;

    machine = vbox->findMachine(name);
    if (machine) {
        /*
          * 虚拟机状态：
          * 1.运行
          * 2.挂起
          * */
        switch (machine->state()) {
            case VBox::MachineState::Running:
            case VBox::MachineState::Paused: {
                /*code*/
                session = vboxClient->session();
                machine->lockMachine(session, VBox::LockType::Shared);

                mutablemachine = session->machine();
                if (mutablemachine) {
                    /*The machine must be in the Running or Paused state
                     * ,otherwise the operation will fail*/
                    progress = mutablemachine->saveState();
                    progress->waitForCompletion(-1);
                }

                session->unlockMachine();
                machine->Release();
            }
                break;

            default:
                machine->Release();
                callbackdata = "Virtual machine state is not Paused or running!";
                return false;
        }

    } else {
        /*虚拟机找不到*/
        callbackdata = "Virtual machine does not exist!";
        machine->Release();
        return false;
    }

    return true;
}

/*
 * 虚拟机重启，传入参数：虚拟机名字
 * */
bool QiVCA::restart(const string &_vmname, string &callbackdata) {

    vca_toCode code;
    std::u16string name = code.to_utf16(_vmname);

    VBox::COMPtr<VBox::ISession> session;
    VBox::COMPtr<VBox::IMachine> machine;

    machine = vbox->findMachine(name);
    if (machine) {
        /*
         * 虚拟机状态
         * 1.running
         * */
        switch (machine->state()) {
            case VBox::MachineState::Running: {
                /*code*/
                session = vboxClient->session();
                machine->lockMachine(session, VBox::LockType::Shared);

                session->console()->reset();
                session->unlockMachine();
                machine->Release();
            }
                break;

            default:
                machine->Release();
                callbackdata = "Virtual machine state is not running!";

                return false;
        }
    } else {
        machine->Release();
        callbackdata = "Virtual machine does not exist!";
        std::cout << "machine can not fond" << endl;
        return false;
    }

    return true;
}

/*
 * 创建空磁盘
 * 1.磁盘名字
 * 1.磁盘类型， VDI; VDH; VMDK
 * 2.磁盘基础路径
 * 3.磁盘大小, 1024L*1024*1024*10 = 10GB
 *
 * */
bool QiVCA::makeFloatDisk(const string &disknameorid,
                          const string &diskformat,
                          const string &basefolder,
                          long int size,
                          string &callbackdata) {

    VBox::COMPtr<VBox::IMedium> disk;
    VBox::COMPtr<VBox::IProgress> progress;
    vca_toCode code;

    string logicdisk_storagepath;
    if (strcmp(diskformat.c_str(), "VDI") == 0) {
        logicdisk_storagepath = basefolder + "/" + disknameorid + "/" + disknameorid + ".vdi";

    } else if (strcmp(diskformat.c_str(), "VHD") == 0) {
        logicdisk_storagepath = basefolder + "/" + disknameorid + "/" + disknameorid + ".vhd";

    } else if (strcmp(diskformat.c_str(), "VMDK") == 0) {
        logicdisk_storagepath = basefolder + "/" + disknameorid + "/" + disknameorid + ".vmdk";
    } else {
        callbackdata = "Virtual disk format is incorrect !";
        return false;
    }

    disk = vbox->createMedium(code.to_utf16(diskformat), code.to_utf16(logicdisk_storagepath),
                              VBox::AccessMode::ReadWrite,
                              VBox::DeviceType::HardDisk);
    if (disk) {

        std::vector<VBox::MediumVariant> variant;
        variant.push_back(VBox::MediumVariant::Standard);
        variant.push_back(VBox::MediumVariant::Fixed);

        progress = disk->createBaseStorage(size, variant);
        while (!progress->completed()) {

            continue;
            std::cout << "percent-->" << progress->percent() << endl;
        }
        //disk id
        callbackdata = code.to_utf8(disk->id());

    } else {
        callbackdata = "Disk already exists ！";
        return false;
    }

    return true;
}

/*
 * 给虚拟机附加磁盘
 * 1.虚拟机名字
 * 2.磁盘名字
 * 3.磁盘类型
 * 4.磁盘控制器类型:IDE;SATA;SCSI;Floppy;SAS;USB;PCIe
 * 5.磁盘基础路径
 * 6.slot,磁盘位置(1, 2, 3, 4)
 * */
bool QiVCA::addDiskToVm(const string &vmname,
                        const string &diskname,
                        const string &diskformat,
                        const string &storagecontroller,
                        const string &basefolder,
                        int slot,
                        string &callbackdata) {

    VBox::COMPtr<VBox::ISession> session;
    VBox::COMPtr<VBox::IMachine> machine;
    VBox::COMPtr<VBox::IMachine> mutableMachine;
    VBox::COMPtr<VBox::IMedium> disk;
    VBox::COMPtr<VBox::IStorageController> controller;
    vca_toCode code;
    std::u16string name = code.to_utf16(vmname);

    if (!((slot > 0) && (slot < 5))) {
        callbackdata = "slot should be :1, 2, 3, 4 !";
        return false;
    }

    string diskfullpath;
    if (strcmp(diskformat.c_str(), "VDI") == 0) {
        diskfullpath = basefolder + "/" + diskname + "/" + diskname + ".vdi";

    } else if (strcmp(diskformat.c_str(), "VHD") == 0) {
        diskfullpath = basefolder + "/" + diskname + "/" + diskname + ".vhd";

    } else if (strcmp(diskformat.c_str(), "VMDK") == 0) {
        diskfullpath = basefolder + "/" + diskname + "/" + diskname + ".vmdk";
    } else {
        callbackdata = "Virtual disk format is incorrect !";
        return false;
    }

    machine = vbox->findMachine(name);
    if (machine) {
        /*开始给VM附加磁盘*/
        session = vboxClient->session();
        machine->lockMachine(session, VBox::LockType::Write);
        mutableMachine = session->machine();

        disk = vbox->openMedium(code.to_utf16(diskfullpath), VBox::DeviceType::HardDisk,
                                VBox::AccessMode::ReadWrite, true);
        if (disk) {
            if (storagecontroller == storageBus[0]) {
                controller = mutableMachine->getStorageControllerByName(u"IDE");
            } else if (storagecontroller == storageBus[1]) {
                controller = mutableMachine->getStorageControllerByName(u"SATA");

            } else if (storagecontroller == storageBus[2]) {
                controller = mutableMachine->getStorageControllerByName(u"SCSI");

            } else if (storagecontroller == storageBus[3]) {
                controller = mutableMachine->getStorageControllerByName(u"Floppy");

            } else if (storagecontroller == storageBus[4]) {
                controller = mutableMachine->getStorageControllerByName(u"SAS");

            } else if (storagecontroller == storageBus[5]) {
                controller = mutableMachine->getStorageControllerByName(u"USB");

            } else if (storagecontroller == storageBus[6]) {
                controller = mutableMachine->getStorageControllerByName(u"PCIe");
            } else {
                callbackdata = " Wrong type of disk controller !";
                machine->Release();
                return false;
            }

            /*start attach disk*/
            try {

                mutableMachine->attachDevice(controller->name(), slot, 0, VBox::DeviceType::HardDisk, disk);
                /*
                 * 支持热插拔设备
                 * */
                mutableMachine->setHotPluggableForDevice(controller->name(), slot, 0, true);

                callbackdata = diskfullpath;

            } catch (const VBox::COMError &err) {
                auto errorInfo = VBox::currentError();
                auto vboxError = errorInfo->QueryInterface<VBox::IVirtualBoxErrorInfo>();
                if (vboxError) {
                    std::cout << "err :" << err.what() << endl;
                    while (vboxError) {
                        auto meg = vboxError->text();
                        for (auto ch : meg) {
                            printf("%c", ch);
                        }
                        printf("\n");
                        vboxError = vboxError->next();
                    }

                    callbackdata = err.what();

                    return false;
                }
            }

            mutableMachine->saveSettings();
            session->unlockMachine();
            machine->Release();

            return true;

        } else {
            callbackdata = " Disk read error  !";
            machine->Release();
            return false;
        }

    } else {
        callbackdata = "Virtual machine does not exist !";
        machine->Release();
        return false;
    }

}

/*
 * destroy disk without attach
 * 1.
 * */
bool QiVCA::destroyFloatDiskWithoutAttach(const string &diskuuid, string &callbackdata) {
    VBox::COMPtr<VBox::ISession> session;
    VBox::COMPtr<VBox::IMachine> machine;
    VBox::COMPtr<VBox::IMachine> mutableMachine;
    VBox::COMPtr<VBox::IMedium> disk;
    vca_toCode code;
    std::u16string diskNameOrId_ = code.to_utf16(diskuuid);

    std::vector<VBox::COMPtr<VBox::IMedium>> disks;
    std::vector<VBox::COMPtr<VBox::IMedium>>::iterator iter_d;


    disks = vbox->hardDisks();
    iter_d = disks.begin();
    while (iter_d != disks.end()){
        disk = *iter_d;

        if ((disk->id() == diskNameOrId_) || (disk->name() == diskNameOrId_)){
            disk->deleteStorage();
            callbackdata = code.to_utf8(disk->name());
            break;
        }

        iter_d++;
    }


    return true;

}

/*
 * 给虚拟机detach磁盘
 * 1.虚拟机名字
 * 2.diskname
 * 3.slot,磁盘位置(1, 2, 3, 4)
 * */
bool QiVCA::detachDiskFromVm(const string &vmname,
                             int slot,
                             string &callbackdata) {

    VBox::COMPtr<VBox::ISession> session;
    VBox::COMPtr<VBox::IMachine> machine;
    VBox::COMPtr<VBox::IMachine> mutableMachine;
    VBox::COMPtr<VBox::IMedium> disk;
    VBox::COMPtr<VBox::IStorageController> controller;
    vca_toCode code;
    std::u16string name = code.to_utf16(vmname);

    if (!((slot > 0) && (slot < 5))) {
        callbackdata = "slot should be :1, 2, 3, 4 !";
        return false;
    }
    machine = vbox->findMachine(name);

    if (machine) {
        std::cout<<"find machine!"<<endl;
        /*开始给VM附加磁盘*/
        session = vboxClient->session();
        machine->lockMachine(session, VBox::LockType::Shared);
        mutableMachine = session->machine();

        controller = mutableMachine->getStorageControllerByName(u"SATA");

        /*detach disk*/
        try {
            mutableMachine->detachDevice(controller->name(), slot, 0);

        } catch (const VBox::COMError &err) {

            auto errorInfo = VBox::currentError();
            auto vboxError = errorInfo->QueryInterface<VBox::IVirtualBoxErrorInfo>();
            if (vboxError) {
                std::cout << "err :" << err.what() << endl;
                while (vboxError) {
                    auto meg = vboxError->text();
                    for (auto ch : meg) {
                        printf("%c", ch);
                    }
                    printf("\n");
                    vboxError = vboxError->next();
                }
            }
            callbackdata = err.what();

            return false;
        }

        mutableMachine->saveSettings();
        session->unlockMachine();
        machine->Release();

        return true;
    }
}

/*
 *获取虚拟机的信息，传入参数：_vmname
 * */
bool QiVCA::getVmInfo(const string &_vmname,
                      string &vminfo,
                      string &callbackdata) {

    /*
     * test make disk
     * */
    Json::Value jsonarray;
    Json::Value jsonarraydisk;
    Json::Value jsonarraynetwork;
    Json::Value jsonarraygraphics;
    Json::Value jsonarrayusb;
    Json::Value jsonarraysharefolder;

    vca_toCode code;
    std::u16string name = code.to_utf16(_vmname);

    VBox::COMPtr<VBox::IMachine> machine;
    map<const string, const string> vmmap;

    VBox::COMPtr<VBox::IMedium> disk;
    VBox::COMPtr<VBox::IMediumAttachment> attach;
    VBox::COMPtr<VBox::IStorageController> satacontroller;

    std::vector<VBox::COMPtr<VBox::IMediumAttachment>> medium;
    std::vector<VBox::COMPtr<VBox::IMediumAttachment>>::iterator iter;

    machine = vbox->findMachine(name);
    /*
     * disk: uuid string
     * */
    map<const string, const string> diskmap;
    medium = machine->getMediumAttachmentsOfController(u"SATA");
    iter = medium.begin();

    while (iter != medium.end()){
        diskmap.clear();
        attach = *iter;
#if 0
        std::cout << "port:" << attach->port() << endl;
        std::cout << "hotPlug:" << attach->hotPluggable() << endl;
        std::cout << "controller type:" << code.to_utf8(attach->controller()) << endl;
        std::cout << "format type:" << code.to_utf8(attach->medium()->format()) << endl;
        std::cout << "name:" << code.to_utf8(attach->medium()->name()) << endl;
        std::cout << "uuid:" << code.to_utf8(attach->medium()->id()) << endl;
        std::cout << "logical size:" << attach->medium()->logicalSize() << endl;
        std::cout << "physics size:" << attach->medium()->size() << endl;
        std::cout << "parent name:" << code.to_utf8(attach->medium()->parent()->name()) << endl;
        std::cout << "parent id:" << code.to_utf8(attach->medium()->parent()->id()) << endl;
#endif
        diskmap.insert(pair<string, string>("name", code.to_utf8(attach->medium()->name())));
        diskmap.insert(pair<string, string>("uuid", code.to_utf8(attach->medium()->id())));
        diskmap.insert(pair<string, string>("port", to_string(attach->port())));
        diskmap.insert(pair<string, string>("hotplug", to_string(attach->hotPluggable())));
        diskmap.insert(pair<string, string>("controller", code.to_utf8(attach->controller())));
        diskmap.insert(pair<string, string>("format", code.to_utf8(attach->medium()->format())));
        diskmap.insert(pair<string, string>("logical_size", to_string(attach->medium()->logicalSize())));
        diskmap.insert(pair<string, string>("physics_size", to_string(attach->medium()->size())));

        if (attach->medium()->parent() == nullptr){
            diskmap.insert(pair<string, string>("parent_name",  "0"));

        } else{
            diskmap.insert(pair<string, string>("parent_name",  code.to_utf8(attach->medium()->parent()->name())));
            diskmap.insert(pair<string, string>("parent_id", code.to_utf8(attach->medium()->parent()->id())));
        }

        jsonarraydisk.append(code.mapTojsonstr(diskmap));

        iter ++;
    }
    /*
     * net: type;mac
     * */
    VBox::COMPtr<VBox::INetworkAdapter> adapter_0;
    VBox::COMPtr<VBox::INetworkAdapter> adapter_1;
    VBox::COMPtr<VBox::INetworkAdapter> adapter_2;
    VBox::COMPtr<VBox::INetworkAdapter> adapter_3;

    adapter_0 = machine->getNetworkAdapter(0);
    adapter_1 = machine->getNetworkAdapter(1);
    adapter_2 = machine->getNetworkAdapter(2);
    adapter_3 = machine->getNetworkAdapter(3);

    map<const string, const string> netmap_0;
    map<const string, const string> netmap_1;
    map<const string, const string> netmap_2;
    map<const string, const string> netmap_3;

    if(adapter_0->enabled()){
        if(adapter_0->attachmentType() == VBox::NetworkAttachmentType::Bridged){
            netmap_0.insert(pair<string, string>("nettype", "Bridged"));

        }
        if(adapter_0->attachmentType() == VBox::NetworkAttachmentType::NAT){
            netmap_0.insert(pair<string, string>("nettype", "NAT"));
        }

        netmap_0.insert(pair<string, string>("VIF", code.to_utf8(adapter_0->bridgedInterface())));
        netmap_0.insert(pair<string, string>("MAC", code.to_utf8(adapter_0->MACAddress())));
        jsonarraynetwork.append(code.mapTojsonstr(netmap_0));
    }

    if(adapter_1->enabled()){
        if(adapter_1->attachmentType() == VBox::NetworkAttachmentType::Bridged){
            netmap_1.insert(pair<string, string>("nettype", "Bridged"));

        }
        if(adapter_1->attachmentType() == VBox::NetworkAttachmentType::NAT){
            netmap_1.insert(pair<string, string>("nettype", "NAT"));
        }

        netmap_1.insert(pair<string, string>("VIF", code.to_utf8(adapter_1->bridgedInterface())));
        netmap_1.insert(pair<string, string>("MAC", code.to_utf8(adapter_1->MACAddress())));
        jsonarraynetwork.append(code.mapTojsonstr(netmap_1));
    }
    if(adapter_2->enabled()){
        if(adapter_2->attachmentType() == VBox::NetworkAttachmentType::Bridged){
            netmap_2.insert(pair<string, string>("nettype", "Bridged"));

        }
        if(adapter_2->attachmentType() == VBox::NetworkAttachmentType::NAT){
            netmap_2.insert(pair<string, string>("nettype", "NAT"));
        }

        netmap_2.insert(pair<string, string>("VIF", code.to_utf8(adapter_2->bridgedInterface())));
        netmap_2.insert(pair<string, string>("MAC", code.to_utf8(adapter_2->MACAddress())));
        jsonarraynetwork.append(code.mapTojsonstr(netmap_2));
    }
    if(adapter_3->enabled()){
        if(adapter_3->attachmentType() == VBox::NetworkAttachmentType::Bridged){
            netmap_3.insert(pair<string, string>("nettype", "Bridged"));

        }
        if(adapter_3->attachmentType() == VBox::NetworkAttachmentType::NAT){
            netmap_3.insert(pair<string, string>("nettype", "NAT"));
        }

        netmap_3.insert(pair<string, string>("VIF", code.to_utf8(adapter_3->bridgedInterface())));
        netmap_3.insert(pair<string, string>("MAC", code.to_utf8(adapter_3->MACAddress())));
        jsonarraynetwork.append(code.mapTojsonstr(netmap_3));
    }


#if 0
    /*
     * sharefolder
     * */
    std::vector<VBox::COMPtr<VBox::ISharedFolder>> folders;
    VBox::COMPtr<VBox::ISharedFolder> folder;
    std::vector<VBox::COMPtr<VBox::ISharedFolder>>::iterator iter_f;
    map<const string, const string> foldermap;

    folders = machine->sharedFolders();
    iter_f = folders.begin();
    while (iter_f != folders.end()){
        std::cout<<"1"<<endl;
        foldermap.clear();
        folder = *iter_f;
//        std::cout<<"123"<<code.to_utf8(folder->name())<<endl;
        std::cout<<to_string(folder->accessible())<<endl;
        std::cout<<code.to_utf8(folder->hostPath())<<endl;
        std::cout<<to_string(folder->autoMount())<<endl;
        std::cout<<code.to_utf8(folder->autoMountPoint())<<endl;
        std::cout<<to_string(folder->writable())<<endl;

//        foldermap.insert(pair<string, string>("name", code.to_utf8(folder->name())));
//        foldermap.insert(pair<string, string>("state", to_string(folder->accessible())));
//        foldermap.insert(pair<string, string>("hostpath", code.to_utf8(folder->hostPath())));
//        foldermap.insert(pair<string, string>("auto", to_string(folder->autoMount())));
//        foldermap.insert(pair<string, string>("mountpoint", code.to_utf8(folder->autoMountPoint())));
//        foldermap.insert(pair<string, string>("writeable", to_string(folder->writable())));
//
//        jsonarraysharefolder.append(code.mapTojsonstr(foldermap));
        iter_f ++;
    }
#endif

    /*
     * Graphics
     * */
    string ram;
    string monitor;
    bool enable3d;
    map<const string, const string> graphics;
    graphics.insert(pair<string, string>("ramsize", to_string(machine->VRAMSize())));
    graphics.insert(pair<string, string>("monitor_count", to_string(machine->monitorCount())));
    graphics.insert(pair<string, string>("enable3d",  to_string(machine->accelerate3DEnabled())));
    jsonarraygraphics.append(code.mapTojsonstr(graphics));


    if (machine) {

        vmmap.insert(pair<string, string>("state", std::to_string(static_cast<int>(machine->state()))));
        vmmap.insert(pair<string, string>("cpucount", std::to_string(static_cast<int>(machine->CPUCount()))));
        vmmap.insert(pair<string, string>("memory", std::to_string(static_cast<int>(machine->memorySize()))));
        vmmap.insert(pair<string, string>("os", code.to_utf8(machine->OSTypeId())));
        vmmap.insert(pair<string, string>("vmuuid", code.to_utf8(machine->hardwareUUID())));
        vmmap.insert(pair<string, string>("disk", jsonarraydisk.toStyledString()));
        vmmap.insert(pair<string, string>("network", jsonarraynetwork.toStyledString()));
        vmmap.insert(pair<string, string>("graphics", jsonarraygraphics.toStyledString()));

        jsonarray.append(code.mapTojsonstr(vmmap));
        //赋值
        vminfo = writer.write(jsonarray);
        machine->Release();

    } else {
        callbackdata = "Virtual machine does not exist!";

        machine->Release();
        return false;
    }

    return true;
}

/*
 * 查找虚拟机快照信息，传入参数虚拟机名字, 字符串形参
 * */
bool QiVCA::getVmSnapshotInfo(const string &_vmname,
                              string &info,
                              string &callbackdata) {

    Json::Value jsonarray;

    vca_toCode code;
    std::u16string name = code.to_utf16(_vmname);
    int snap_count;
    std::u16string snap_path;
    std::u16string snap_current_name;
    std::u16string snap_parent_name;
    map<const string, const string> vmsnap;

    VBox::COMPtr<VBox::IMachine> machine;
    VBox::COMPtr<VBox::ISnapshot> current_snap;
    machine = vbox->findMachine(name);
    if (machine) {
        snap_count = machine->snapshotCount();
        snap_path = machine->snapshotFolder();

        if (snap_count >= 1) {
            /*
             * 虚拟机有快照
             * */
            current_snap = machine->currentSnapshot();
            snap_current_name = current_snap->name();

            if (snap_count >= 2) {
                /*
                 * 虚拟机有父级快照
                 * */
                snap_parent_name = current_snap->parent()->name();

            } else {
                snap_parent_name = u"0";
            }

            vmsnap.insert(pair<string, string>("count", std::to_string(snap_count)));
            vmsnap.insert(pair<string, string>("path", code.to_utf8(snap_path)));
            vmsnap.insert(pair<string, string>("current", code.to_utf8(snap_current_name)));
            if (!snap_parent_name.empty()) {
                vmsnap.insert(pair<string, string>("parent", code.to_utf8(snap_parent_name)));

            } else {
                machine->Release();
                callbackdata = " Failure during virtual machine snapshot data collection !";
                return false;
            }

        } else {
            callbackdata = "Virtual machine has no snapshot !";
            machine->Release();
            return false;
        }
        /*
         * map -> jsonarray
         * */
        jsonarray.append(code.mapTojsonstr(vmsnap)).toStyledString();
        //赋值
        info = writer.write(jsonarray);
        machine->Release();

    } else {
        callbackdata = " Virtual machine does not exist !";
        machine->Release();
        return false;
    }

    return true;
}

/*
 *创建虚拟机快照,传入参数：虚拟机名字, 快照名字
 * */
bool QiVCA::takeSnapshot(const string &_vmname,
                         const string &basicString,
                         string &callbackdata) {

    vca_toCode code;
    std::u16string name = code.to_utf16(_vmname);
    std::u16string snap_name = code.to_utf16(basicString);

    VBox::COMPtr<VBox::ISession> session;
    VBox::COMPtr<VBox::IMachine> machine;
    VBox::COMPtr<VBox::IMachine> mutabmachine;
    VBox::COMPtr<VBox::IProgress> progress;

    machine = vbox->findMachine(name);
    if (machine) {
        session = vboxClient->session();

        switch (machine->state()) {
            case VBox::MachineState::Running: {
                /*code*/
                machine->lockMachine(session, VBox::LockType::Shared);
                mutabmachine = session->machine();

                if (mutabmachine) {
                    basic_string<char16_t> id = mutabmachine->id();

                    /*VM is running*/
                    progress = mutabmachine->takeSnapshot(snap_name, u"vca core!", false, &id);
                    progress->waitForCompletion(-1);

                } else {
                    callbackdata = " Failed to get virtual machine variable object !";
                    session->unlockMachine();
                    machine->Release();
                    return false;
                }
                session->unlockMachine();
                machine->Release();
            }
                break;

            default:
                machine->Release();
                callbackdata = " Virtual state should be running!";
                return false;
        }
    } else {
        callbackdata = " Virtual machine does not exist !";
        machine->Release();
        return false;
    }

    return true;
}

/*
 * 恢复到虚拟机快照，传入参数：虚拟机名字，快照名字
 * */
bool QiVCA::restoreSnapshot(const string &_vmname,
                            const string &basicString,
                            string &callbackdata) {

    try {
        vca_toCode code;
        std::u16string name = code.to_utf16(_vmname);
        std::u16string snap_name = code.to_utf16(basicString);

        VBox::COMPtr<VBox::ISession> session;
        VBox::COMPtr<VBox::IMachine> machine;
        VBox::COMPtr<VBox::IMachine> mutabmachine;
        VBox::COMPtr<VBox::IProgress> progress;
        VBox::COMPtr<VBox::ISnapshot> snap;

        machine = vbox->findMachine(name);
        if (machine) {

            switch (machine->state()) {
                case VBox::MachineState::Starting:
                case VBox::MachineState::Stopping:
                case VBox::MachineState::Paused:
                case VBox::MachineState::Saving:
                case VBox::MachineState::Running: {
                    /*
                     * 恢复快照时候，虚拟机必须是关机或者saved状态，否则先执行挂起到磁盘再恢复
                     * */
                    string data;
                    bool ret = vmSleep(_vmname, data);
                    if (!ret) {
                        callbackdata = data;
                        return false;
                    }
                }
                    break;

                case VBox::MachineState::PoweredOff:
                case VBox::MachineState::Saved: {
                    /*code*/
                    session = vboxClient->session();
                    machine->lockMachine(session, VBox::LockType::Shared);
                    mutabmachine = session->machine();

                    if (mutabmachine) {
                        /*VM is stoped*/
                        snap = mutabmachine->findSnapshot(snap_name);
                        if (snap) {
                            progress = mutabmachine->restoreSnapshot(snap);
                            progress->waitForCompletion(-1);

                        } else {
                            callbackdata = "Virtual machine snapshot does not exist !";
                            session->unlockMachine();
                            machine->Release();
                            return false;
                        }

                    } else {
                        callbackdata = " Failed to get virtual machine variable object !";
                        session->unlockMachine();
                        machine->Release();
                        return false;
                    }
                    session->unlockMachine();
                    machine->Release();
                }
                    break;

                default:
                    machine->Release();
                    callbackdata = "Virtual machine state is not PoweredOff or saved, and powerOff failed!";
                    return false;
            }
        } else {
            callbackdata = "Virtual machine does not exist !";
            machine->Release();
            return false;
        }

        return true;

    } catch (const VBox::COMError &err) {
        auto errorInfo = VBox::currentError();
        auto vboxError = errorInfo->QueryInterface<VBox::IVirtualBoxErrorInfo>();
        if (vboxError) {
            std::cout << "err :" << err.what() << endl;
            while (vboxError) {
                auto meg = vboxError->text();
                for (auto ch : meg) {
                    printf("%c", ch);
                }
                printf("\n");
                vboxError = vboxError->next();
            }
        }
    }
}

/*
 * 删除快照, 传参:
 * 1.虚拟机名字，
 * 2.快照名字<请必须逆向删除快照>
 */
bool QiVCA::deleteSnapshot(const string &_vmname,
                           const string &basicString,
                           string &callbackdata) {

    vca_toCode code;
    std::u16string name = code.to_utf16(_vmname);
    std::u16string snap_name = code.to_utf16(basicString);

    VBox::COMPtr<VBox::ISession> session;
    VBox::COMPtr<VBox::IMachine> machine;
    VBox::COMPtr<VBox::IMachine> mutabmachine;
    VBox::COMPtr<VBox::IProgress> progress;
    VBox::COMPtr<VBox::ISnapshot> snap;

    machine = vbox->findMachine(name);
    if (machine) {

        switch (machine->state()) {
            case VBox::MachineState::PoweredOff:
            case VBox::MachineState::Running:
            case VBox::MachineState::Saved: {
                /*code*/
                session = vboxClient->session();
                machine->lockMachine(session, VBox::LockType::Shared);
                mutabmachine = session->machine();

                if (mutabmachine) {
                    snap = mutabmachine->findSnapshot(snap_name);
                    if (snap) {
                        progress = mutabmachine->deleteSnapshot(snap->id());
                        progress->waitForCompletion(-1);
                        session->unlockMachine();
                        machine->Release();

                    } else {
                        callbackdata = "Virtual machine snapshot does not exist !";
                        session->unlockMachine();
                        machine->Release();
                        return false;
                    }

                } else {
                    callbackdata = " Failed to get virtual machine variable object !";
                    session->unlockMachine();
                    machine->Release();
                    return false;
                }
            }
                break;

            default:
                machine->Release();
                callbackdata = "Virtual machine state is not PoweredOff or saved or running!";

                return false;
        }
    } else {
        callbackdata = "Virtual machine does not exist !";
        machine->Release();
        return false;
    }

    return true;
}

/*
 * 完整克隆虚拟机，传入参数:
 * 1.虚拟机名字
 * 2.基础路径
 * 3.新克隆虚拟机的名字
 * 4.是否保留原mac，是否保留
 * 5.是否保留原硬件uuid
 * 6.是否保留原磁盘名字
 * */
bool QiVCA::cloneFullVM(const string &_vmname,
                        const string &basefolder,
                        const string &target_vmname,
                        bool keepmacs,
                        bool keephwuuids,
                        bool keepdiskname,
                        string &callbackdata) {

    vca_toCode code;
    std::u16string name = code.to_utf16(_vmname);
    std::u16string target_name = code.to_utf16(target_vmname);
    std::u16string base_folder = code.to_utf16(basefolder);

    VBox::COMPtr<VBox::IMachine> machine;
    VBox::COMPtr<VBox::IMachine> targetmachine;
    VBox::COMPtr<VBox::IProgress> progress;


    machine = vbox->findMachine(name);
    if (machine) {
        /*
         *虚拟机状态必须是关机或者挂起到磁盘
         * */
        callbackdata = code.to_utf8(machine->id());


        switch (machine->state()) {
            case VBox::MachineState::PoweredOff:
            case VBox::MachineState::Saved: {


                /*code*/
                auto _p = vbox->composeMachineFilename(target_name, u"", u"", base_folder);

                targetmachine = vbox->createMachine(_p, target_name, std::vector<std::u16string>(),
                                                    machine->OSTypeId(), u"");
                if (targetmachine) {
                    targetmachine->saveSettings();
                } else {
                    callbackdata = "Virtual machine creation failed !";
                    machine->Release();
                    return false;
                }

                /*
                 * 克隆虚拟机的所有网卡信息，硬件uuid，磁盘名称
                 * */
                std::vector<VBox::CloneOptions> options;
                if (keepmacs) {
                    /*
                     * 为克隆保留mac地址
                     * */
                    options.push_back(VBox::CloneOptions::KeepAllMACs);

                }
                if (keephwuuids) {
                    /*
                     * 为克隆保留硬件uuid
                     * */
                    options.push_back(VBox::CloneOptions::KeepHwUUIDs);

                }
                if (keepdiskname) {
                    /*
                     * 为克隆保留磁盘名字
                     * */
                    options.push_back(VBox::CloneOptions::KeepDiskNames);

                }
                /*
                 * 克隆虚拟机的全部状态
                 * */
                progress = machine->cloneTo(targetmachine, VBox::CloneMode::AllStates, options);
                while (!progress->completed()) {

                    continue;
                    std::cout << "percent-->" << progress->percent() << endl;

                }
                vbox->registerMachine(targetmachine);
                machine->Release();
            }
                break;

            case VBox::MachineState::Null:
            case VBox::MachineState::Teleported:
            case VBox::MachineState::Aborted:
            case VBox::MachineState::Running:
            case VBox::MachineState::Paused:
            case VBox::MachineState::Stuck:
            case VBox::MachineState::Teleporting:
            case VBox::MachineState::LiveSnapshotting:
            case VBox::MachineState::Starting:
            case VBox::MachineState::Stopping:
            case VBox::MachineState::Saving:
            case VBox::MachineState::Restoring:
            case VBox::MachineState::TeleportingPausedVM:
            case VBox::MachineState::TeleportingIn:
            case VBox::MachineState::FaultTolerantSyncing:
            case VBox::MachineState::DeletingSnapshotOnline:
            case VBox::MachineState::DeletingSnapshotPaused:
            case VBox::MachineState::OnlineSnapshotting:
            case VBox::MachineState::RestoringSnapshot:
            case VBox::MachineState::DeletingSnapshot:
            case VBox::MachineState::SettingUp:
            case VBox::MachineState::Snapshotting:
                callbackdata = " Virtual machine state does not allow cloning !";
                machine->Release();
                return false;
        }

    } else {
        callbackdata = " Virtual machine not exists !";
        machine->Release();
        return false;
    }

    return 0 == progress->resultCode();
}

/*
 * 链接克隆虚拟机，传入参数：
 * 1.虚拟机名字
 * 2.基础路径,存储路径
 * 3.新虚拟机名字
 * */
bool QiVCA::cloneLinkVM(const string &_vmname,
                        const string &basefolder,
                        const string &target_vmname,
                        string &callbackdata) {

    try {
        vca_toCode code;
        std::u16string name = code.to_utf16(_vmname);
        std::u16string target_name = code.to_utf16(target_vmname);
        std::u16string base_folder = code.to_utf16(basefolder);

        VBox::COMPtr<VBox::IMachine> machine;
        VBox::COMPtr<VBox::IMachine> targetmachine;
        VBox::COMPtr<VBox::IProgress> progress;
        VBox::COMPtr<VBox::IProgress> cloneprogress;

        VBox::COMPtr<VBox::ISession> session;
        VBox::COMPtr<VBox::IMachine> mutabmachine;
        VBox::COMPtr<VBox::ISnapshot> snap;
        VBox::COMPtr<VBox::IMachine> snapmachine;

        machine = vbox->findMachine(name);
        if (machine) {
            /*
             *虚拟机的状态必须是关机或者挂起到磁盘
             * */
            callbackdata = code.to_utf8(machine->id());

            switch (machine->state()) {
                case VBox::MachineState::PoweredOff:
                case VBox::MachineState::Saved: {
                    /*
                     * 链接克隆是基于快照的，首先制作快照。
                     * */
                    session = vboxClient->session();
                    machine->lockMachine(session, VBox::LockType::Shared);
                    mutabmachine = session->machine();

                    if (mutabmachine) {
                        auto id = mutabmachine->id();
                        progress = mutabmachine->takeSnapshot(u"vca_clone_link_2019_12_18", u"vca clone link!", false,
                                                              &id);
                        progress->waitForCompletion(-1);
                        /*
                         * 启动嵌套分页
                         * */
                        mutabmachine->setHWVirtExProperty(VBox::HWVirtExPropertyType::NestedPaging, true);

                    }
                    session->unlockMachine();

                    /*
                     * 将快照克隆至链接虚拟机
                     * */
                    auto _p = vbox->composeMachineFilename(target_name, u"", u"", base_folder);

                    targetmachine = vbox->createMachine(_p, target_name, std::vector<std::u16string>(),
                                                        machine->OSTypeId(), u"");
                    if (targetmachine) {
                        targetmachine->saveSettings();

                        std::vector<VBox::CloneOptions> options;
                        options.push_back(VBox::CloneOptions::Link);

                        snap = machine->findSnapshot(u"vca_clone_link_2019_12_18");
                        snapmachine = snap->machine();
                        cloneprogress = snapmachine->cloneTo(targetmachine, VBox::CloneMode::MachineState, options);

                        while (!cloneprogress->completed()) {
                        }
                        vbox->registerMachine(targetmachine);
                        machine->Release();
                    }
                }
                    break;

                case VBox::MachineState::Null:
                case VBox::MachineState::Teleported:
                case VBox::MachineState::Aborted:
                case VBox::MachineState::Running:
                case VBox::MachineState::Paused:
                case VBox::MachineState::Stuck:
                case VBox::MachineState::Teleporting:
                case VBox::MachineState::LiveSnapshotting:
                case VBox::MachineState::Starting:
                case VBox::MachineState::Stopping:
                case VBox::MachineState::Saving:
                case VBox::MachineState::Restoring:
                case VBox::MachineState::TeleportingPausedVM:
                case VBox::MachineState::TeleportingIn:
                case VBox::MachineState::FaultTolerantSyncing:
                case VBox::MachineState::DeletingSnapshotOnline:
                case VBox::MachineState::DeletingSnapshotPaused:
                case VBox::MachineState::OnlineSnapshotting:
                case VBox::MachineState::RestoringSnapshot:
                case VBox::MachineState::DeletingSnapshot:
                case VBox::MachineState::SettingUp:
                case VBox::MachineState::Snapshotting:
                    callbackdata = " Virtual machine state does not allow cloning !";
                    machine->Release();
                    return false;
            }

        } else {
            callbackdata = "Virtual machine not exists !";
            machine->Release();
            return false;
        }

        return 0 == cloneprogress->resultCode();

    } catch (const VBox::COMError &err) {

        auto errorInfo = VBox::currentError();
        auto vboxError = errorInfo->QueryInterface<VBox::IVirtualBoxErrorInfo>();
        if (vboxError) {
            std::cout << "err :" << err.what();
            while (vboxError) {
                auto mes = vboxError->text();
                for (auto c : mes) {
                    printf("%c", c);
                }
                vboxError = vboxError->next();
            }
        }
    }
}

/*
 * 从images创建虚拟机,至少传入参数:
 * 1.虚拟机名字;
 * 2.系统类型ID;
 * 3.初始cpu数量;
 * 4.初始内存大小MB;
 * 5.基础路径
 * 6.系统盘全路径;
 * 7.磁盘接口类型;
 * 8.错误信息
 * */
bool QiVCA::createVMfromSystemdisk(const string &_vmname,
                                   const string &ostypeid,
                                   int cpucount,
                                   int memorysize,
                                   const string &basefolder,
                                   const string &diskpath,
                                   const string &storagebustype,
                                   string &callbackdata) {
    try {

        vca_toCode code;
        std::u16string name = code.to_utf16(_vmname);
        std::u16string path = code.to_utf16(diskpath);
        std::u16string base_folder = code.to_utf16(basefolder);

        VBox::COMPtr<VBox::ISession> session;
        VBox::COMPtr<VBox::IMachine> machine;
        VBox::COMPtr<VBox::IMachine> mutableMachine;
        VBox::COMPtr<VBox::IMedium> disk;
        VBox::COMPtr<VBox::IStorageController> controller;


        for (const auto &iter : osType) {
            /*
             * 查看系统类型是否符合
             * */
            if (strcmp(ostypeid.c_str(), iter.data()) == 0) {

                std::u16string os_type = code.to_utf16(ostypeid);
                auto _p = vbox->composeMachineFilename(name, u"", u"", base_folder);

                machine = vbox->createMachine(_p, name, std::vector<std::u16string>(0),
                                              os_type, u"");

                if (machine) {
                    /*vm create successful*/
                    callbackdata = code.to_utf8(machine->id());

                    /*vm settings*/
                    machine->set_CPUCount(cpucount);
                    machine->set_OSTypeId(os_type);
                    machine->set_memorySize(memorysize);

                    session = vboxClient->session();
                    vbox->registerMachine(machine);

                    machine->lockMachine(session, VBox::LockType::Write);
                    mutableMachine = session->machine();
                    /*
                     * 启动嵌套分页
                     * */
                    mutableMachine->setHWVirtExProperty(VBox::HWVirtExPropertyType::NestedPaging, true);

                    disk = vbox->openMedium(path, VBox::DeviceType::HardDisk, VBox::AccessMode::ReadWrite, true);
                    if (disk) {
                        disk->refreshState();

                        if (storagebustype == storageBus[0]) {
                            controller = mutableMachine->addStorageController(u"IDE", VBox::StorageBus::IDE);
                        } else if (storagebustype == storageBus[1]) {
                            controller = mutableMachine->addStorageController(u"SATA", VBox::StorageBus::SATA);

                        } else if (storagebustype == storageBus[2]) {
                            controller = mutableMachine->addStorageController(u"SCSI", VBox::StorageBus::SCSI);

                        } else if (storagebustype == storageBus[3]) {
                            controller = mutableMachine->addStorageController(u"Floppy", VBox::StorageBus::Floppy);

                        } else if (storagebustype == storageBus[4]) {
                            controller = mutableMachine->addStorageController(u"SAS", VBox::StorageBus::SAS);

                        } else if (storagebustype == storageBus[5]) {
                            controller = mutableMachine->addStorageController(u"USB", VBox::StorageBus::USB);

                        } else if (storagebustype == storageBus[6]) {
                            controller = mutableMachine->addStorageController(u"PCIe", VBox::StorageBus::PCIe);
                        }

                        mutableMachine->attachDevice(controller->name(), 0, 0, VBox::DeviceType::HardDisk, disk);
                        /*
                         * position: must be in range [1, 4]
                         * */
                        mutableMachine->setBootOrder(1, VBox::DeviceType::HardDisk);
                        mutableMachine->saveSettings();
                        session->unlockMachine();
                        machine->Release();

                    } else {
                        callbackdata = " Virtual machine disk open failed !";

                        session->unlockMachine();
                        disk->Release();
                        machine->Release();

                        return false;
                    }

                } else {
                    callbackdata = "Virtual machine creation failed!";
                    machine->Release();
                    return false;
                }
            }

        }

        return true;

    } catch (const VBox::COMError &err) {

        auto errorInfo = VBox::currentError();
        auto vboxError = errorInfo->QueryInterface<VBox::IVirtualBoxErrorInfo>();
        if (vboxError) {
            std::cout << "err :" << err.what() << endl;
            while (vboxError) {
                auto mes = vboxError->text();
                for (auto c : mes) {
                    printf("%c", c);
                }
                vboxError = vboxError->next();
            }
        }
    }
}

/*
 * 从ISO进行装机，至少传入参数：
 * 1.虚拟机名字;
 * 2.系统类型ID;
 * 3.初始CPU数量;
 * 4.初始内存大小MB;
 * 5.装机磁盘basefolder
 * 6.创建逻辑磁盘大小;
 * 7.ISO全路径;
 * 8.虚拟机磁盘格式化类型;vdi  vhd vmdk
 * 9.磁盘控制器接口类型;
 * 10.是否动态磁盘(动态大小，固定分配) <动态大小时候传参逻辑磁盘大小：建议32GB>
 * 11.装机磁盘路径，外部传入形参
 * 12.错误信息
 * */
bool QiVCA::createVMfromISO(const string &_vmname,
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
                            string &callbackdata) {

    try {

        vca_toCode code;
        std::u16string name = code.to_utf16(_vmname);
        std::u16string iso_path = code.to_utf16(isopath);
        std::u16string base_folder = code.to_utf16(basefolder);

        VBox::COMPtr<VBox::ISession> session;
        VBox::COMPtr<VBox::IMachine> machine;
        VBox::COMPtr<VBox::IMachine> mutableMachine;
        VBox::COMPtr<VBox::IMedium> disk;
        VBox::COMPtr<VBox::IStorageController> diskcontroller;
        VBox::COMPtr<VBox::IStorageController> dvdcontroller;

        VBox::COMPtr<VBox::IProgress> progress;
        VBox::COMPtr<VBox::IMedium> dvd;

        for (const auto &iter : osType) {
            /*
             * 查看系统类型是否符合
             * */
            if (strcmp(ostypeid.c_str(), iter.data()) == 0) {

                std::u16string os_type = code.to_utf16(ostypeid);
                /*
                 * 查看磁盘格式化类型是否符合
                 * */
                for (const auto &iter_l : mediaType) {
                    if (strcmp(diskformat.c_str(), iter_l.data()) == 0) {

                        std::u16string media_type = code.to_utf16(diskformat);
                        /*
                         * 首先在节点上创建虚拟机
                         * */
                        auto _p = vbox->composeMachineFilename(name, u"", u"", base_folder);

                        machine = vbox->createMachine(_p, name, vector<std::u16string>(0), os_type, u"");
                        if (machine) {
                            /*vm create successful*/
                            vmuuid = code.to_utf8(machine->id());
                            /*vm settings*/
                            machine->set_CPUCount(cpucount);
                            machine->set_OSTypeId(os_type);
                            machine->set_memorySize(memorysize);

                            session = vboxClient->session();

                            /*
                             * 将虚拟机注册到vbox
                             * */
                            vbox->registerMachine(machine);
                            machine->lockMachine(session, VBox::LockType::Write);
                            mutableMachine = session->machine();
                            /*
                             * 启动嵌套分页
                             * */
                            mutableMachine->setHWVirtExProperty(VBox::HWVirtExPropertyType::NestedPaging, true);

                            /*
                             * 创建磁盘媒体，支持vdi、vhd、vmdk三种格式的虚拟化磁盘
                             * */
                            std::string logicdisk_storagepath;
                            if (strcmp(diskformat.c_str(), "VDI") == 0) {
                                logicdisk_storagepath = basefolder + "/" + _vmname + "/" + _vmname + ".vdi";

                            } else if (strcmp(diskformat.c_str(), "VHD") == 0) {
                                logicdisk_storagepath = basefolder + "/" + _vmname + "/" + _vmname + ".vhd";

                            } else if (strcmp(diskformat.c_str(), "VMDK") == 0) {
                                logicdisk_storagepath = basefolder + "/" + _vmname + "/" + _vmname + ".vmdk";
                            } else {
                                callbackdata = "Virtual disk format is incorrect !";
                                return false;
                            }

                            disk = vbox->createMedium(media_type, code.to_utf16(logicdisk_storagepath),
                                                      VBox::AccessMode::ReadWrite,
                                                      VBox::DeviceType::HardDisk);
                            if (disk) {

                                std::vector<VBox::MediumVariant> variant;
                                variant.push_back(VBox::MediumVariant::Standard);
                                if (fix) {
                                    /*
                                     * 设置虚拟机的磁盘固定大小，
                                     * */
                                    variant.push_back(VBox::MediumVariant::Fixed);
                                }

                                progress = disk->createBaseStorage(logicdisk_size, variant);

                                progress->waitForCompletion(-1);
                                if (progress->resultCode() != 0) {
                                    /*
                                     * 虚拟机磁盘创建fail
                                     * */
                                    callbackdata = "Virtual machine disk creation failed!";
                                    session->unlockMachine();
                                    machine->Release();
                                    return false;
                                }
                                /*
                                 * 虚拟磁盘的接口类型
                                 * */
                                if (storagebustype == storageBus[0]) {
                                    diskcontroller = mutableMachine->addStorageController(u"IDE",
                                                                                          VBox::StorageBus::IDE);
                                } else if (storagebustype == storageBus[1]) {
                                    diskcontroller = mutableMachine->addStorageController(u"SATA",
                                                                                          VBox::StorageBus::SATA);

                                } else if (storagebustype == storageBus[2]) {
                                    diskcontroller = mutableMachine->addStorageController(u"SCSI",
                                                                                          VBox::StorageBus::SCSI);

                                } else if (storagebustype == storageBus[3]) {
                                    diskcontroller = mutableMachine->addStorageController(u"Floppy",
                                                                                          VBox::StorageBus::Floppy);

                                } else if (storagebustype == storageBus[4]) {
                                    diskcontroller = mutableMachine->addStorageController(u"SAS",
                                                                                          VBox::StorageBus::SAS);

                                } else if (storagebustype == storageBus[5]) {
                                    diskcontroller = mutableMachine->addStorageController(u"USB",
                                                                                          VBox::StorageBus::USB);

                                } else if (storagebustype == storageBus[6]) {
                                    diskcontroller = mutableMachine->addStorageController(u"PCIe",
                                                                                          VBox::StorageBus::PCIe);
                                }
                                /*
                                 * iso的接口类型全部是IDE
                                 * */
                                dvdcontroller = mutableMachine->addStorageController(u"IDE", VBox::StorageBus::IDE);
                                /*
                                 * 打开iso
                                 * */
                                dvd = vbox->openMedium(iso_path, VBox::DeviceType::DVD,
                                                       VBox::AccessMode::ReadOnly, true);
                                if (dvd) {
                                    mutableMachine->attachDevice(dvdcontroller->name(), 0, 0, VBox::DeviceType::DVD,
                                                                 dvd);

                                    mutableMachine->attachDevice(diskcontroller->name(), 0, 0,
                                                                 VBox::DeviceType::HardDisk, disk);
                                    mutableMachine->mountMedium(dvdcontroller->name(), 0, 0, dvd, true);

                                    mutableMachine->setBootOrder(1, VBox::DeviceType::DVD);
                                    mutableMachine->setBootOrder(2, VBox::DeviceType::HardDisk);

                                    mutableMachine->saveSettings();
                                    session->unlockMachine();
                                } else {
                                    callbackdata = " DVD device open failed !";
                                    return false;
                                }

                            } else {
                                /*
                                 * 创建disk失败
                                 * */
                                callbackdata = "Virtual machine disk creation failed!";
                                session->unlockMachine();
                                disk->Release();
                                machine->Release();
                                return false;
                            }
                        } else {
                            callbackdata = "Virtual machine creation failed!";
                            machine->Release();
                            return false;
                        }
                    }
                }
            }
        }
        return true;

    } catch (const VBox::COMError &err) {
        auto errorInfo = VBox::currentError();
        auto vboxError = errorInfo->QueryInterface<VBox::IVirtualBoxErrorInfo>();
        if (vboxError) {
            std::cout << "err :" << err.what() << endl;
            while (vboxError) {
                auto msg = vboxError->text();
                std::cout << "msg:" << msg.data() << endl;
                vboxError = vboxError->next();
            }
        }
    }
}

/*
 * vm删除，传入参数：
 * 1.虚拟机名字
 * 2.删除模式（1、2、3、4）
 * */
bool QiVCA::deleteVM(const string &_vmname, int _type, string &callbackdata) {
    try {
        vca_toCode code;
        std::u16string name = code.to_utf16(_vmname);

        VBox::COMPtr<VBox::IProgress> progress;
        VBox::COMPtr<VBox::IMachine> machine;

        machine = vbox->findMachine(name);
        if (machine) {

            switch (machine->state()) {
                case VBox::MachineState::PoweredOff: {
                    if (4 == _type) {
                        auto mediums = machine->unregister(VBox::CleanupMode::Full);
                        progress = machine->deleteConfig(mediums);

                        progress->waitForCompletion(-1);

                    } else if (3 == _type) {
                        auto mediums = machine->unregister(VBox::CleanupMode::DetachAllReturnHardDisksOnly);
                        progress = machine->deleteConfig(mediums);
                        progress->waitForCompletion(-1);

                    } else if (2 == _type) {
                        auto mediums = machine->unregister(VBox::CleanupMode::DetachAllReturnNone);
                        progress = machine->deleteConfig(mediums);
                        progress->waitForCompletion(-1);

                    } else if (1 == _type) {
                        auto mediums = machine->unregister(VBox::CleanupMode::UnregisterOnly);
                        progress = machine->deleteConfig(mediums);
                        progress->waitForCompletion(-1);

                    } else {
                        callbackdata = "delete type is err!";
                    }

#ifdef Q_WS_WIN
                    for (auto medium:mediums) {
                           progress = medium->deleteStorage();
                           progress->waitForCompletion(-1);
                        }
#endif
                    machine->Release();
                }
                    break;

                default:
                    callbackdata = "VM should be poweroff!";
                    return false;
            }

        } else {
            /*虚拟机不存在*/
            callbackdata = "Virtual machine does not exist!";
            machine->Release();
            return false;
        }

        return true;

    } catch (const VBox::COMError &err) {

        auto errorInfo = VBox::currentError();
        auto vboxError = errorInfo->QueryInterface<VBox::IVirtualBoxErrorInfo>();
        if (vboxError) {
            cout << "err :" << err.what();
            while (vboxError) {
                auto mes = vboxError->text();
                for (auto c : mes) {
                    printf("%c", c);
                }
                printf("\n");
                vboxError = vboxError->next();
            }
        }
    }
}

/*
 * 虚拟机导出接口，传入参数：
 * 1.虚拟机名字
 * 2.是否脱去mac
 * 3.导出格式：ovf-2.0、ovf-1.0、ovf-0.9
 * 4.导出路径(该路径必须存在)
 * 5.行参：导出成功后文件全路径
 * */
bool QiVCA::exportVM(const string &_vmname,
                     bool keepmac,
                     const string &ovafileType,
                     const string &exportpath,
                     string &ovapath,
                     string &callbackdata) {

    try {
        vca_toCode code;
        std::u16string name = code.to_utf16(_vmname);
        std::u16string _format = code.to_utf16(ovafileType);
        std::u16string _path = code.to_utf16(exportpath + "/" + _vmname + ".ova");

        VBox::COMPtr<VBox::IProgress> progress;
        VBox::COMPtr<VBox::IMachine> machine;
        VBox::COMPtr<VBox::IAppliance> app;
        VBox::COMPtr<VBox::IVirtualSystemDescription> desc;

        machine = vbox->findMachine(name);
        if (machine) {
            app = vbox->createAppliance();

            desc = machine->exportTo(app, name);

            std::vector<VBox::ExportOptions> options;
            if (!keepmac) {
                options.push_back(VBox::ExportOptions::StripAllMACs);
            }

            options.push_back(VBox::ExportOptions::ExportDVDImages);
            options.push_back(VBox::ExportOptions::CreateManifest);

            progress = app->write(_format, options, _path);
            while (!progress->completed()) {
                continue;
                std::cout << "percent-->" << progress->percent() << endl;
            }

            if (0 == progress->resultCode()) {
                ovapath = exportpath + "/" + _vmname + ".ova";
                return true;

            } else {
                ovapath = "";
                callbackdata = " Write error during export!";
                return false;
            }

        } else {
            /*虚拟机不存在*/
            callbackdata = "Virtual machine does not exist!";
            machine->Release();
            return false;
        }

    } catch (const VBox::COMError &err) {

        auto errorInfo = VBox::currentError();
        auto vboxError = errorInfo->QueryInterface<VBox::IVirtualBoxErrorInfo>();
        if (vboxError) {
            std::cout << "err :" << err.what();
            while (vboxError) {
                auto mes = vboxError->text();
                for (auto c : mes) {
                    printf("%c", c);
                }
                printf("\n");
                vboxError = vboxError->next();
            }
        }
    }
}

/*
 * 导入虚拟机,兼容vmware; 传入参数：
 * 1.导入后生成虚拟机名字
 * 2.虚拟机ova系统磁盘路径
 * 3.导入是否保留原虚拟机MAC
 * 4.导入虚拟机的系统类型
 * 5.导入后CPU核数
 * 6.导入后内存大小
 * 7.导入后保留一张网卡的类型,默认开启桥接网卡，建议此项为false。导入成功后再设置网卡
 * 8.basefolder基础路径
 * */
bool QiVCA::importVM(const string &vmname,
                     const string &ovapath,
                     bool keepmac,
                     const string &ostypeid,
                     int cpucount,
                     int memory,
                     bool netup,
                     const string &basefolder,
                     string &callbackdata) {

    try {
        vca_toCode code;
        const std::u16string ova_path = code.to_utf16(ovapath);
        const std::u16string os_type = code.to_utf16(ostypeid);
        std::u16string vm_name = code.to_utf16(vmname);
        const std::u16string cpu_count = code.to_utf16(to_string(cpucount));
        const std::u16string vm_memory = code.to_utf16(to_string(memory));
        const std::u16string base_folder = code.to_utf16(basefolder);
        const std::u16string vm_storage_path = code.to_utf16(basefolder + "/" + vmname + "/" + vmname + ".vbox");
        const std::u16string disk_name = vm_name.append(u".vmdk");

        VBox::COMPtr<VBox::IAppliance> app;
        VBox::COMPtr<VBox::IProgress> i_progress;
        VBox::COMPtr<VBox::IProgress> r_progress;
        VBox::COMPtr<VBox::IMachine> machine;
        std::vector<VBox::COMPtr<VBox::IVirtualSystemDescription>> desc;

        std::vector<VBox::ImportOptions> options;

        options.push_back(VBox::ImportOptions::ImportToVDI);
        if (keepmac) {
            options.push_back(VBox::ImportOptions::KeepAllMACs);
        }
        /*
         * 创建appliance
         * */
        app = vbox->createAppliance();
        /*
         * 执行read()函数读取×××.ova，只要磁盘格式正确，read()函数就可以自动解析文件
         * */
        r_progress = app->read(ova_path);
        r_progress->waitForCompletion(-1);

        if (r_progress->resultCode() < 0) {
            callbackdata = " Error reading ova file during import!";
            machine->Release();
            return false;
        }
        /*
         * 调用interpret()函数，可以设置虚拟机导入的设置信息
         * */
        app->interpret();
        /*
         * 输出interpret过程中的warn信息
         * */
        std::vector<std::u16string> warn = app->getWarnings();
        std::vector<std::u16string>::iterator iter;
        iter = warn.begin();
        while (iter != warn.end()) {
            std::cout << code.to_utf8(*iter) << endl;
        }
        /*
         * 获取SystemDescription的集合
         * */
        desc = app->virtualSystemDescriptions();
        std::vector<bool> enabled;
        std::vector<std::u16string> VBoxValues;
        std::vector<std::u16string> extraConfigValues;

        for (auto &tmp : desc) {
            /*
             * 输入设置信息, 所有的设置项目可以从getDescription()函数中输出得到，可设置VBoxValues和extraConfigValues
             * */
            enabled.emplace_back(true);
            enabled.emplace_back(true);
            enabled.emplace_back(true);
            enabled.emplace_back(true);
            enabled.emplace_back(true);
            enabled.emplace_back(true);
            enabled.emplace_back(true);
            enabled.emplace_back(true);
            enabled.emplace_back(true);
            enabled.emplace_back(true);
            enabled.emplace_back(true);
            enabled.emplace_back(true);
            enabled.emplace_back(true);
            enabled.emplace_back(true);
            enabled.emplace_back(true);

            VBoxValues.emplace_back(os_type);     //系统类型
            VBoxValues.emplace_back(vm_name);        //虚拟机名字
            VBoxValues.emplace_back(u"/");
            VBoxValues.emplace_back(vm_storage_path);      //虚拟机xml文件存储路径
            VBoxValues.emplace_back(base_folder);     //虚拟机存储basefolder
            VBoxValues.emplace_back(cpu_count);         //默认cpu数目
            VBoxValues.emplace_back(vm_memory);      //默认内存大小
            VBoxValues.emplace_back(u"0");
            VBoxValues.emplace_back(u"");
            VBoxValues.emplace_back(u"3");
            VBoxValues.emplace_back(u"");
            VBoxValues.emplace_back(u"PIIX4");
            VBoxValues.emplace_back(u"PIIX4");
            VBoxValues.emplace_back(u"AHCI");
            VBoxValues.emplace_back(disk_name);       //虚拟机磁盘名字，导入后自动转化为vdi格式磁盘

            extraConfigValues.emplace_back(u"");
            extraConfigValues.emplace_back(u"");
            extraConfigValues.emplace_back(u"");
            extraConfigValues.emplace_back(u"");
            extraConfigValues.emplace_back(u"");
            extraConfigValues.emplace_back(u"");
            extraConfigValues.emplace_back(u"");
            extraConfigValues.emplace_back(u"");
            extraConfigValues.emplace_back(u"");
            if (netup) {
                extraConfigValues.emplace_back(u"slot=0;type=Bridged");
            } else {
                extraConfigValues.emplace_back(u"");
            }
            extraConfigValues.emplace_back(u"");
            extraConfigValues.emplace_back(u"");
            extraConfigValues.emplace_back(u"");
            extraConfigValues.emplace_back(u"");
            extraConfigValues.emplace_back(u"controller=13;channel=0");

            tmp->setFinalValues(enabled, VBoxValues, extraConfigValues);

        }

        i_progress = app->importMachines(options);
        while (!i_progress->completed()) {
            continue;
            std::cout << "percent-->" << i_progress->percent() << endl;
        }

        machine->Release();
        return i_progress->resultCode() >= 0;

    } catch (const VBox::COMError &err) {

        auto errorInfo = VBox::currentError();
        auto vboxError = errorInfo->QueryInterface<VBox::IVirtualBoxErrorInfo>();
        if (vboxError) {
            std::cout << "err :" << err.what();
            while (vboxError) {
                auto mes = vboxError->text();
                for (auto c : mes) {
                    printf("%c", c);
                }
                printf("\n");
                vboxError = vboxError->next();
            }
        }
    }
}

/*
 * 克隆虚拟机时候，建议使用
    auto evSource = vbox->eventSource();
    auto listener = evSource->createListener();
    evSource->registerListener(listener, {VBox::VBoxEventType::Any}, false);

    std::thread event_thread([evSource, listener]() {
        for ( ;; ) {
            auto event = evSource->getEvent(listener, 500);
            if (!event)
                continue;

            std::wcout << L"Got event type " << static_cast<int>(event->type())
                       << std::endl;
            if (auto stateChangedEvent = event->QueryInterface<VBox::IStateChangedEvent>()) {
                std::wcout << L"    State change: "
                           << static_cast<int>(stateChangedEvent->state())
                           << std::endl;
            }
            evSource->eventProcessed(listener, event);
        }
    });

    event_thread.detach();
*/
