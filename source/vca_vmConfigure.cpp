//
// Created by root on 2019/11/26.
//
#include <ctime>
#include <utility>
#include "../include/vca_toCode.h"
#include "../include/vca_type.h"
#include "../include/vca_vmConfigure.h"
#include "../include/libvbox_interfaces.h"

//VBox::COMPtr<VBox::IVirtualBox> vbox) : Listener(std::move(vbox)
vcaVmConfigure::vcaVmConfigure() {

    vboxClient_c = VBox::virtualBoxClient();
    vbox_c = vboxClient_c->virtualBox();
    machines_c = vbox_c->machines();
}

vcaVmConfigure::~vcaVmConfigure() {

    vbox_c->Release();
    vboxClient_c->Release();

}

/*
 * 虚拟机设置指定网卡为桥接模式，传入参数:
 * 1.虚拟机名字
 * 2.虚拟机网卡slot号(0/1/2/3)
 * 3.节点主机的网卡接口名字
 * */
bool vcaVmConfigure::setBridge(const string &_vmname,
                               int slot,
                               const string &hostpci,
                               string &callbackdata) {
    try {

        vca_toCode code;
        std::u16string name = code.to_utf16(_vmname);
        std::u16string pci_name = code.to_utf16(hostpci);

        VBox::COMPtr<VBox::ISession> session;
        VBox::COMPtr<VBox::IMachine> machine;
        VBox::COMPtr<VBox::IMachine> mutabmachine;
        VBox::COMPtr<VBox::INetworkAdapter> adapter;
        if (!((slot >= 0) && (slot <= 3))) {
            callbackdata = "slot should between 0~3";
            return false;
        }
        machine = vbox_c->findMachine(name);
        if (machine) {

            session = vboxClient_c->session();
            machine->lockMachine(session, VBox::LockType::Shared);
            mutabmachine = session->machine();

            adapter = mutabmachine->getNetworkAdapter(slot);
            if (adapter) {
                adapter->set_bridgedInterface(pci_name);
                adapter->set_attachmentType(VBox::NetworkAttachmentType::Bridged);
                adapter->set_enabled(true);

                mutabmachine->saveSettings();
                session->unlockMachine();
                machine->Release();

            } else {
                /*
                 * 找不到网络适配器，释放虚拟机
                 * */
                callbackdata = " Virtual machine adapter does not exist!";
                machine->Release();
                return false;
            }

        } else {
            callbackdata = " Virtual machine does not exist!";
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
 * 虚拟机添加桥接网卡，传入参数:
 * 1.虚拟机名字
 * 2.虚拟机网卡slot号(0/1/2/3)
 * 3.节点主机的网卡接口名字
 * */
bool vcaVmConfigure::addBridge(const string &_vmname,
                               int slot,
                               const string &hostpci,
                               string &callbackdata) {
    vca_toCode code;
    std::u16string name = code.to_utf16(_vmname);
    std::u16string pci_name = code.to_utf16(hostpci);

    VBox::COMPtr<VBox::ISession> session;
    VBox::COMPtr<VBox::IMachine> machine;
    VBox::COMPtr<VBox::IMachine> mutabmachine;
    VBox::COMPtr<VBox::INetworkAdapter> adapter;
    if (!((slot >= 0) && (slot <= 3))) {
        callbackdata = "slot should between 0~3";
        return false;
    }

    machine = vbox_c->findMachine(name);
    if (machine) {
        switch (machine->state()) {
            case VBox::MachineState::PoweredOff: {
                session = vboxClient_c->session();
                machine->lockMachine(session, VBox::LockType::Shared);
                mutabmachine = session->machine();

                adapter = mutabmachine->getNetworkAdapter(slot);
                if (adapter) {
                    adapter->set_bridgedInterface(pci_name);
                    adapter->set_attachmentType(VBox::NetworkAttachmentType::Bridged);
                    adapter->set_enabled(true);

                    mutabmachine->saveSettings();
                    session->unlockMachine();
                    machine->Release();

                } else {
                    /*
                     * 找不到网络适配器，释放虚拟机
                     * */
                    callbackdata = " Virtual machine adapter does not exist!";
                    machine->Release();
                    return false;
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
            case VBox::MachineState::Saved:
                callbackdata = " Virtual machine must be poweroff!";
                machine->Release();
                return false;
        }

    } else {
        /*
         * 找不到
         * */
        callbackdata = " Virtual machine does not exist!";
        machine->Release();
        return false;
    }

    return true;
}

/*
 * 设置网卡nat，传入参数:
 * 1.虚拟机名字
 * 2.slot序列。
 * */
bool vcaVmConfigure::setNat(const string &_vmname,
                            int slot,
                            string &callbackdata) {
    try {

        vca_toCode code;
        std::u16string name = code.to_utf16(_vmname);

        VBox::COMPtr<VBox::ISession> session;
        VBox::COMPtr<VBox::IMachine> machine;
        VBox::COMPtr<VBox::IMachine> mutabmachine;
        VBox::COMPtr<VBox::INetworkAdapter> adapter;

        if (!((slot >= 0) && (slot <= 3))) {
            callbackdata = "slot should between 0~3";
            return false;
        }

        machine = vbox_c->findMachine(name);
        if (machine) {

            session = vboxClient_c->session();
            machine->lockMachine(session, VBox::LockType::Shared);

            mutabmachine = session->machine();

            adapter = mutabmachine->getNetworkAdapter(slot);
            if (adapter) {

                // adapter->set_adapterType(VBox::NetworkAdapterType::I82540EM);   //
                adapter->set_attachmentType(VBox::NetworkAttachmentType::NAT);

                adapter->set_enabled(true);

                mutabmachine->saveSettings();
                session->unlockMachine();
                machine->Release();

            } else {
                /*
                 * 找不到网络适配器，释放虚拟机
                 * */
                callbackdata = " Virtual machine adapter does not exist!";
                machine->Release();
                return false;
            }
        } else {
            /*
             * 找不到虚拟机
             * */
            callbackdata = " Virtual machine does not exist!";
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
 * 添加网卡nat，传入参数:
 * 1.虚拟机名字
 * 2.slot序列。
 * */
bool vcaVmConfigure::addNat(const string &_vmname,
                            int slot,
                            string &callbackdata) {
    try {

        vca_toCode code;
        std::u16string name = code.to_utf16(_vmname);

        VBox::COMPtr<VBox::ISession> session;
        VBox::COMPtr<VBox::IMachine> machine;
        VBox::COMPtr<VBox::IMachine> mutabmachine;
        VBox::COMPtr<VBox::INetworkAdapter> adapter;

        if (!((slot >= 0) && (slot <= 3))) {
            callbackdata = "slot should between 0~3";
            return false;
        }

        machine = vbox_c->findMachine(name);
        if (machine) {

            switch (machine->state()) {
                case VBox::MachineState::PoweredOff: {
                    session = vboxClient_c->session();
                    machine->lockMachine(session, VBox::LockType::Shared);

                    mutabmachine = session->machine();

                    adapter = mutabmachine->getNetworkAdapter(slot);
                    if (adapter) {

                        adapter->set_attachmentType(VBox::NetworkAttachmentType::NAT);

                        adapter->set_enabled(true);

                        mutabmachine->saveSettings();
                        session->unlockMachine();
                        machine->Release();

                    } else {
                        /*
                         * 找不到网络适配器，释放虚拟机
                         * */
                        callbackdata = " Virtual machine adapter does not exist!";
                        machine->Release();
                        return false;
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
                case VBox::MachineState::Saved:
                    callbackdata = " Virtual machine must be poweroff!";
                    machine->Release();
                    return false;

            }

        } else {
            callbackdata = " Virtual machine does not exist!";
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
 * 设置虚拟机网卡mac,传入参数：
 * 1.虚拟机名字
 * 2.slot号
 * 3.新mac地址
 * */
bool vcaVmConfigure::setAdaptorMAC(const string &_vmname,
                                   int slot,
                                   const string &mac,
                                   string &callbackdata) {
    try {
        vca_toCode code;
        std::u16string name = code.to_utf16(_vmname);
        std::u16string pci_mac = code.to_utf16(mac);

        VBox::COMPtr<VBox::ISession> session;
        VBox::COMPtr<VBox::IMachine> machine;
        VBox::COMPtr<VBox::IMachine> mutabmachine;
        VBox::COMPtr<VBox::INetworkAdapter> adapter;
        if (!((slot >= 0) && (slot <= 3))) {
            callbackdata = "slot should between 0~3";
            return false;
        }
        machine = vbox_c->findMachine(name);
        if (machine) {

            switch (machine->state()) {
                case VBox::MachineState::PoweredOff: {
                    session = vboxClient_c->session();
                    machine->lockMachine(session, VBox::LockType::Shared);

                    mutabmachine = session->machine();
                    adapter = mutabmachine->getNetworkAdapter(slot);
                    if (adapter) {
                        /*
                         * 设置虚拟机指定slot的mac
                         * */
                        adapter->set_enabled(true);
                        adapter->set_MACAddress(pci_mac);

                        mutabmachine->saveSettings();
                        session->unlockMachine();
                        machine->Release();

                    } else {
                        callbackdata = " Virtual machine adapter does not exist!";
                        machine->Release();
                        return false;
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
                case VBox::MachineState::Saved:
                    callbackdata = " Virtual machine must be poweroff!";
                    machine->Release();
                    return false;
            }

        } else {
            /*找不到*/
            callbackdata = " Virtual machine does not exist!";
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
 *删除虚拟机网卡,传入参数:
 * 1.虚拟机名字
 * 2.slot号(0/1/2/3)
 * */
bool vcaVmConfigure::disableAdaptor(const string &_vmname,
                                    int slot,
                                    string &callbackdata) {
    try {

        vca_toCode code;
        std::u16string name = code.to_utf16(_vmname);

        VBox::COMPtr<VBox::ISession> session;
        VBox::COMPtr<VBox::IMachine> machine;
        VBox::COMPtr<VBox::IMachine> mutabmachine;
        VBox::COMPtr<VBox::INetworkAdapter> adapter;
        if (!((slot >= 0) && (slot <= 3))) {
            callbackdata = "slot should between 0~3";
            return false;
        }
        machine = vbox_c->findMachine(name);
        if (machine) {

            switch (machine->state()) {
                case VBox::MachineState::PoweredOff: {
                    session = vboxClient_c->session();
                    machine->lockMachine(session, VBox::LockType::Shared);
                    mutabmachine = session->machine();

                    adapter = mutabmachine->getNetworkAdapter(slot);
                    if (adapter) {
                        /*
                         * 通过slot号，禁用网络适配器
                         * */
                        adapter->set_enabled(false);
                        mutabmachine->saveSettings();
                        session->unlockMachine();
                        machine->Release();

                    } else {
                        /*找不到网络是配器*/
                        callbackdata = " Virtual machine adapter does not exist!";
                        machine->Release();
                        return false;
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
                case VBox::MachineState::Saved:
                    callbackdata = " Virtual machine must be poweroff!";
                    machine->Release();
                    return false;
            }

        } else {
            callbackdata = " Virtual machine does not exist!";
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
 *  给虚拟机创建共享文件夹,(不用时候尽量删除共享文件夹)传入参数:
 *  1.虚拟机名字
 *  2.文件夹名字
 *  3.服务器文件路径
 *  4.是否有写权限
 *  5.是否开机自动挂载
 *  6.虚拟机内挂载路径
 * */
bool vcaVmConfigure::createSharefolder(const string &_vmname,
                                       const string &foldername,
                                       const string &externalpath,
                                       bool writeable,
                                       bool automount,
                                       const string &internalpath,
                                       string &callbackdata) {

    try {

        vca_toCode code;
        std::u16string name = code.to_utf16(_vmname);
        std::u16string folder = code.to_utf16(foldername);
        std::u16string externpath = code.to_utf16(externalpath);
        std::u16string internpath_;

        if(internalpath.empty()){
            internpath_ = u"";
        } else{
            internpath_ = code.to_utf16(internalpath);
        }

        VBox::COMPtr<VBox::ISession> session;
        VBox::COMPtr<VBox::IMachine> machine;
        VBox::COMPtr<VBox::IMachine> mutabmachine;

        machine = vbox_c->findMachine(name);
        if (machine) {
            session = vboxClient_c->session();
            machine->lockMachine(session, VBox::LockType::Shared);
            mutabmachine = session->machine();
            /*
             * 创建共享文件夹
             * */
            mutabmachine->createSharedFolder(folder, externpath, writeable, automount, internpath_);
            mutabmachine->saveSettings();
            session->unlockMachine();
            machine->Release();

        } else {
            callbackdata = " Virtual machine does not exist!";
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
 * 删除共享文件夹，传入参数:
 * 1.虚拟机名字
 * 2.共享文件夹名字
 * */
bool vcaVmConfigure::removeSharedFolder(const string &_vmname,
                                        const string &foldername,
                                        string &callbackdata) {

    try {

        vca_toCode code;
        std::u16string name = code.to_utf16(_vmname);
        std::u16string folder = code.to_utf16(foldername);

        VBox::COMPtr<VBox::ISession> session;
        VBox::COMPtr<VBox::IMachine> machine;
        VBox::COMPtr<VBox::IMachine> mutabmachine;

        machine = vbox_c->findMachine(name);
        if (machine) {

            session = vboxClient_c->session();
            machine->lockMachine(session, VBox::LockType::Shared);
            mutabmachine = session->machine();
            /*
             * 删除共享文件夹
             * */
            mutabmachine->removeSharedFolder(folder);
            mutabmachine->saveSettings();
            session->unlockMachine();
            machine->Release();

        } else {
            callbackdata = " Virtual machine does not exist!";
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
 *设置虚拟机的RDP,监听ip:port
 * 传入参数:
 * 1.虚拟机名字
 * 2.监听ip
 * 3.port
 * 4.是否允许多链接
 * 5.是否重用单链接
 * */
bool vcaVmConfigure::setSimpleRemoteDisplay(const string &vmname,
                                            const string &ip,
                                            int port,
                                            bool allowMultiConnect,
                                            bool reuseSingleConnect,
                                            string &callbackdata) {

    try {
        vca_toCode code;
        std::u16string name = code.to_utf16(vmname);
        std::u16string ip_l = code.to_utf16(ip);

        VBox::COMPtr<VBox::ISession> session;
        VBox::COMPtr<VBox::IMachine> machine;
        VBox::COMPtr<VBox::IMachine> mutabmachine;
        VBox::COMPtr<VBox::IVRDEServer> rdpserver;

        machine = vbox_c->findMachine(name);
        if (machine) {
            switch (machine->state()) {
                case VBox::MachineState::PoweredOff: {
                    /*code*/
                    session = vboxClient_c->session();

                    machine->lockMachine(session, VBox::LockType::Shared);
                    mutabmachine = session->machine();

                    if (mutabmachine) {
                        rdpserver = mutabmachine->VRDEServer();
                        rdpserver->set_enabled(true);

                        rdpserver->set_allowMultiConnection(allowMultiConnect != 0);

                        rdpserver->set_reuseSingleConnection(reuseSingleConnect != 0);

                        rdpserver->set_authTimeout(6000);

                        rdpserver->setVRDEProperty(u"TCP/Ports", code.to_utf16(to_string(port)));
                        rdpserver->setVRDEProperty(u"TCP/Address", ip_l);

                        mutabmachine->saveSettings();
                        session->unlockMachine();
                        machine->Release();

                    } else {
                        callbackdata = " Failed to get mutable machine object !";
                        session->unlockMachine();
                        machine->Release();
                        return false;
                    }
                }
                    break;

                default:
                    callbackdata = " Virtual machine should be poweroff !";
                    return false;
            }

        } else {
            callbackdata = " Virtual machine does not exist !";
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
 * 获取rdp链接信息,传入参数
 * 1.虚拟机名字
 * 2.外部行参vdredata
 * */
bool vcaVmConfigure::getRdpInfo(const string &vmname,
                                string &vdredata,
                                string &callbackdata) {
    try {
        vca_toCode code;
        std::u16string name = code.to_utf16(vmname);

        std::u16string client_ip;
        std::int32_t port;

        bool state;
        std::u16string user;
        std::u16string domain;
        std::uint32_t client_count;
        std::u16string client_name;
        std::uint32_t version;
        std::uint32_t encryption;

        std::int64_t begin;
        std::int64_t end;
        std::int64_t bytesSent;
        std::int64_t bytesSentTotal;
        std::int64_t bytesReceived;
        std::int64_t bytesReceivedTotal;

        Json::Value jsonarray;
        map<const string, const string> vrde;

        VBox::COMPtr<VBox::ISession> session;
        VBox::COMPtr<VBox::IMachine> machine;
        VBox::COMPtr<VBox::IMachine> mutabmachine;
        VBox::COMPtr<VBox::IVRDEServerInfo> vdeserverInfo;
        machine = vbox_c->findMachine(name);
        if (machine) {

            /*
             * 开机时候注意虚拟机状态，以下状态下不可进行开机操作:
             * 1.开机中，运行中，关机中
             * 2.保存状态中，恢复运行中
             * 3.快照中，删除快照中，恢复快照中
             * */
            switch (machine->state()) {
                case VBox::MachineState::Running: {
                    /*code*/
                    session = vboxClient_c->session();
                    machine->lockMachine(session, VBox::LockType::Shared);

                    /*
                     * 从session->console()获取rdp服务端的信息
                    * */
                    vdeserverInfo = session->console()->VRDEServerInfo();

                    state = vdeserverInfo->active();    //链接是否活跃
                    if (state) {
                        vrde.insert(pair<const string, const string>("vdeState", "1"));

                    } else {
                        vrde.insert(pair<const string, const string>("vdeState", "0"));
                        return false;
                    }

                    port = vdeserverInfo->port();   //端口
                    client_ip = vdeserverInfo->clientIP();   //client ip
                    if (!code.to_utf8(client_ip).empty()) {
                        vrde.insert(pair<const string, const string>("clientIP", code.to_utf8(client_ip)));

                    } else {
                        vrde.insert(pair<const string, const string>("clientIP", "0"));

                    }

                    if (port) {
                        vrde.insert(pair<const string, const string>("port", to_string(port)));

                    } else {
                        vrde.insert(pair<const string, const string>("port", "0"));

                    }
                    user = vdeserverInfo->user();   //user
                    domain = vdeserverInfo->domain();   //domain
                    if (!code.to_utf8(user).empty()) {
                        vrde.insert(pair<const string, const string>("user", code.to_utf8(user)));

                    } else {
                        vrde.insert(pair<const string, const string>("user", "0"));

                    }

                    if (!code.to_utf8(domain).empty()) {
                        vrde.insert(pair<const string, const string>("domain", code.to_utf8(domain)));

                    } else {
                        vrde.insert(pair<const string, const string>("domain", "0"));

                    }

                    client_count = vdeserverInfo->numberOfClients();     //链接数量
                    client_name = vdeserverInfo->clientName();    //链接端名字
                    version = vdeserverInfo->clientVersion();     //链接端版本
                    if (client_count) {
                        vrde.insert(pair<const string, const string>("connectCount", to_string(client_count)));

                    } else {
                        vrde.insert(pair<const string, const string>("connectCount", "0"));

                    }

                    if (!code.to_utf8(client_name).empty()) {
                        vrde.insert(pair<const string, const string>("clientName", code.to_utf8(client_name)));

                    } else {
                        vrde.insert(pair<const string, const string>("clientName", "0"));

                    }

                    if (version) {
                        vrde.insert(pair<const string, const string>("clientVersion", to_string(version)));

                    } else {
                        vrde.insert(pair<const string, const string>("clientVersion", "0"));

                    }

                    encryption = vdeserverInfo->encryptionStyle();
                    if (encryption) {
                        vrde.insert(pair<const string, const string>("encryptionType", to_string(encryption)));

                    } else {
                        vrde.insert(pair<const string, const string>("encryptionType", "0"));

                    }

                    begin = vdeserverInfo->beginTime();
                    end = vdeserverInfo->endTime();
                    if (begin) {
                        vrde.insert(pair<const string, const string>("beginTime", to_string(begin)));

                    } else {
                        vrde.insert(pair<const string, const string>("beginTime", "0"));

                    }

                    if (end) {
                        vrde.insert(pair<const string, const string>("endTime", to_string(end)));

                    } else {
                        vrde.insert(pair<const string, const string>("endTime", "0"));

                    }

                    bytesReceived = vdeserverInfo->bytesReceived();
                    bytesReceivedTotal = vdeserverInfo->bytesReceivedTotal();
                    if (bytesReceived) {
                        vrde.insert(pair<const string, const string>("bytesReceived", to_string(bytesReceived)));

                    } else {
                        vrde.insert(pair<const string, const string>("bytesReceived", "0"));

                    }
                    if (bytesReceivedTotal) {
                        vrde.insert(pair<const string, const string>("bytesReceivedTotal", to_string(bytesReceivedTotal)));

                    } else {
                        vrde.insert(pair<const string, const string>("bytesReceivedTotal", "0"));

                    }

                    bytesSent = vdeserverInfo->bytesSent();
                    bytesSentTotal = vdeserverInfo->bytesSentTotal();
                    if (bytesSent) {
                        vrde.insert(pair<const string, const string>("bytesSent", to_string(bytesSent)));

                    } else {
                        vrde.insert(pair<const string, const string>("bytesSent", "0"));

                    }
                    if (bytesSentTotal) {
                        vrde.insert(pair<const string, const string>("bytesSentTotal", to_string(bytesSentTotal)));

                    } else {
                        vrde.insert(pair<const string, const string>("bytesSentTotal", "0"));

                    }

                    jsonarray.append(code.mapTojsonstr(vrde));
                    vdredata = writer.write(jsonarray);

                    session->unlockMachine();
                    machine->Release();
                }
                break;

                default:
                    callbackdata = " Virtual machine should be running!";
                    machine->Release();
            }

        } else {
            callbackdata = " Virtual machine does not exist !";
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
 *  关闭Rdp远程桌面，传入参数：
 *  1.虚拟机名字
 *
 * */
bool vcaVmConfigure::disableRDP(const string &_vmname, string &callbackdata) {

    try {
        vca_toCode code;
        std::u16string name = code.to_utf16(_vmname);

        VBox::COMPtr<VBox::ISession> session;
        VBox::COMPtr<VBox::IMachine> machine;
        VBox::COMPtr<VBox::IMachine> mutabmachine;
        VBox::COMPtr<VBox::IVRDEServer> rdpserver;

        machine = vbox_c->findMachine(name);
        if (machine) {

            session = vboxClient_c->session();

            machine->lockMachine(session, VBox::LockType::Shared);
            mutabmachine = session->machine();

            if (mutabmachine) {

                rdpserver = mutabmachine->VRDEServer();
                rdpserver->set_enabled(false);
                mutabmachine->saveSettings();
                session->unlockMachine();
                machine->Release();

            } else {
                callbackdata = " Failed to get mutable machine object !";
                session->unlockMachine();
                machine->Release();
                return false;
            }

        } else {
            callbackdata = " Virtual machine does not exist !";
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
 * 重置虚拟机cpu数量,传入参数
 * 1.虚拟机名字
 * 2.cpu数
 * */
bool vcaVmConfigure::resetCpuCount(const string &_vmname,
                                   int cpuCount,
                                   string &callbackdata) {

    try {
        vca_toCode code;
        std::u16string name = code.to_utf16(_vmname);

        VBox::COMPtr<VBox::ISession> session;
        VBox::COMPtr<VBox::IMachine> machine;
        VBox::COMPtr<VBox::IMachine> mutabmachine;

        if (!((cpuCount >= 1) && (cpuCount <= 4))) {
            callbackdata = "cpucount : 1 ~ 4 !";
            return false;
        }

        machine = vbox_c->findMachine(name);
        if (machine) {

            switch (machine->state()) {
                case VBox::MachineState::PoweredOff: {
                    session = vboxClient_c->session();

                    machine->lockMachine(session, VBox::LockType::Shared);
                    mutabmachine = session->machine();

                    if (mutabmachine) {
                        mutabmachine->set_CPUCount(static_cast<uint32_t >(cpuCount));
                        mutabmachine->saveSettings();
                    }
                    session->unlockMachine();
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
                case VBox::MachineState::Saved:
                    callbackdata = " Virtual machine should be poweroff !";
                    machine->Release();
                    return false;
            }
        } else {
            callbackdata = " Virtual machine does not exist !";
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
 * 重置虚拟机内存大小，传入参数
 * 1. 虚拟机名字
 * 2. 内存大小（0～8192）MB
 * */
bool vcaVmConfigure::resetMemSize(const string &_vmname,
                                  int memorySize,
                                  string &callbackdata) {
    try {
        vca_toCode code;
        std::u16string name = code.to_utf16(_vmname);

        VBox::COMPtr<VBox::ISession> session;
        VBox::COMPtr<VBox::IMachine> machine;
        VBox::COMPtr<VBox::IMachine> mutabmachine;

        if (!((memorySize >= 0) && (memorySize <= 8192))) {
            callbackdata = "memorySize : 0 ~8192MB !";
            return false;
        }

        machine = vbox_c->findMachine(name);
        if (machine) {

            switch (machine->state()) {
                case VBox::MachineState::PoweredOff: {
                    session = vboxClient_c->session();

                    machine->lockMachine(session, VBox::LockType::Shared);
                    mutabmachine = session->machine();

                    if (mutabmachine) {
                        mutabmachine->set_memorySize(static_cast<uint32_t >(memorySize));

                        mutabmachine->saveSettings();
                    }

                    session->unlockMachine();
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
                case VBox::MachineState::Saved:
                    callbackdata = " Virtual machine should be poweroff !";
                    machine->Release();
                    return false;
            }
        } else {
            /*找不到*/
            callbackdata = " Virtual machine does not exist !";
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
 * 显卡参数设置，显存大小;监视器数量;显卡控制器;3D加速
 * 1.显存大小ramSize(0~128)MB
 * 2.监视器数量monitorCount(1~8)
 * 3.显卡控制器cardController(VBoxVGA;VMSVGA;VBoxSVGA)
 * 4.3D加速enable3D
 * */
bool vcaVmConfigure::resetGraphics(const string &vmname,
                                   int ramSize,
                                   int monitorCount,
                                   const string &cardController,
                                   bool enable3D,
                                   string &callbackdata) {

    try {

        vca_toCode code;
        std::u16string name = code.to_utf16(vmname);
        std::u16string cardctl = code.to_utf16(cardController);

        VBox::COMPtr<VBox::ISession> session;
        VBox::COMPtr<VBox::IMachine> machine;
        VBox::COMPtr<VBox::IMachine> mutabmachine;

        if (!((ramSize >= 0) && (ramSize <= 128))) {
            /*
             * 显存大小范围0~128MB
             * */
            callbackdata = " ramSize : 0 ~ 128MB !";
            return false;
        }

        if (!((monitorCount >= 1) && (monitorCount <= 8))) {
            /*
             * 监视器数量1~8个
             * */
            callbackdata = " monitorCount : 1 ~ 8MB !";
            return false;
        }

        machine = vbox_c->findMachine(name);
        if (machine) {
            switch (machine->state()) {
                case VBox::MachineState::PoweredOff: {
                    session = vboxClient_c->session();
                    machine->lockMachine(session, VBox::LockType::Shared);
                    mutabmachine = session->machine();

                    if (mutabmachine) {

                        if (enable3D) {
                            /*
                             * 开启3D加速
                             * */
                            mutabmachine->set_accelerate3DEnabled(true);
                        }
                        /*
                         * 设置显存大小
                         * */
                        mutabmachine->set_VRAMSize(static_cast<uint32_t >(ramSize));
                        /*
                         * 设置显示器个数
                         * */
                        mutabmachine->set_monitorCount(static_cast<uint32_t >(monitorCount));

                        for (const auto &iter_l : graphicsCtrlType) {
                            if (strcmp(cardController.c_str(), iter_l.data()) == 0) {
                                /*
                                 * 设置显卡控制器
                                 * */
                                if (strcmp(iter_l.data(), "VBoxSVGA") == 0)
                                    mutabmachine->set_graphicsControllerType(VBox::GraphicsControllerType::VBoxSVGA);

                                if (strcmp(iter_l.data(), "VBoxVGA") == 0)
                                    mutabmachine->set_graphicsControllerType(VBox::GraphicsControllerType::VBoxVGA);

                                if (strcmp(iter_l.data(), "VMSVGA") == 0)
                                    mutabmachine->set_graphicsControllerType(VBox::GraphicsControllerType::VMSVGA);

                            }
                        }
                        /*
                         * 默认开启2D视频加速
                         * */
                        mutabmachine->set_accelerate2DVideoEnabled(true);

                        mutabmachine->saveSettings();
                        session->unlockMachine();
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
                case VBox::MachineState::Saved:
                    callbackdata = " Virtual machine should be poweroff !";
                    machine->Release();
                    return false;
            }
        } else {
            /*找不到*/
            callbackdata = " Virtual machine does not exist !";
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
 * 设置录屏幕功能配置项目,传参<注意，仅仅在关机状态下进行设置>
 * 1.虚拟机名字
 * 2.屏幕监视器id（一般0～7）
 * 3.设置并生效（虚拟机关机状态下）
 * 4.视频文件存储全路径（.webm格式）默认路径/var/lib/vcloud/vedio
 * 5.像素Width<320~2880>
 * 6.像素Height<200~1800>
 * 7.帧率1～30 fps
 * 8.比特率 （102~1024kbps）
 * 9.音质 （低->0;中->1; 高->2）
 * 注：如果有一次设置多次下载，文件不会自动重名覆盖，新文件名字为centos01 1-1575005096*.webm->centos01 1-1575005096-2019-11-29T06-19-01-550400000Z.webm
 * */
bool vcaVmConfigure::setRecordingScreenConfig(const string &vmname,
                                              int screenid,
                                              bool on,
                                              const string &basepath,
                                              int width,
                                              int height,
                                              int fps,
                                              int kbps,
                                              int audioquality,
                                              string &videostoragepath,
                                              string &callbackdata) {
    try {
        vca_toCode code;
        std::u16string name = code.to_utf16(vmname);

        std::u16string vediopath;
        VBox::COMPtr<VBox::ISession> session;
        VBox::COMPtr<VBox::IMachine> machine;
        VBox::COMPtr<VBox::IMachine> mutabmachine;
        VBox::COMPtr<VBox::IRecordingSettings> record;
        VBox::COMPtr<VBox::IRecordingScreenSettings> screen;

        if (!basepath.empty()) {
            vediopath = code.to_utf16(basepath);

        } else {
            vediopath = code.to_utf16("/var/lib/vcloud/video");

        }

        if (!((screenid >= 0) && (screenid <= 7))) {
            callbackdata = "screenid : 0 ~ 7!";
            return false;
        }

        if (!((width >= 320) && (width <= 2880))) {
            callbackdata = "width : 320 ~ 2880!";
            return false;
        }

        if (!((height >= 200) && (height <= 1800))) {
            callbackdata = "height : 200 ~ 1800!";
            return false;
        }

        if (!((fps >= 1) && (fps <= 30))) {
            callbackdata = "fps : 1 ~ 30!";
            return false;
        }

        if (!((kbps >= 102) && (kbps <= 1024))) {
            callbackdata = "kbps : 102 ~ 1024!";
            return false;
        }

        if (!((audioquality >= 0) && (audioquality <= 2))) {
            callbackdata = "audioquality : 0 ~ 2!";
            return false;
        }

        machine = vbox_c->findMachine(name);
        if (machine) {

            switch (machine->state()) {
                case VBox::MachineState::PoweredOff: {
                    time_t nowtime = time(nullptr);
                    session = vboxClient_c->session();
                    machine->lockMachine(session, VBox::LockType::Shared);
                    mutabmachine = session->machine();

                    if (mutabmachine) {
                        record = mutabmachine->recordingSettings();
                        /*
                         * 目前支持一次进行一个屏幕的录制
                         * */
                        screen = record->getScreenSettings(static_cast<uint32_t >(screenid));
                        if (on) {
                            /*
                             * 设置并生效
                             * */
                            record->set_enabled(true);
                            screen->set_enabled(true);
                        }

                        /*
                         * 设置存储视频文件
                         * */
                        screen->set_destination(VBox::RecordingDestination::File);
                        std::u16string filepath = vediopath.append(u"/").append(name).append(u"-").append(
                                code.to_utf16(to_string(nowtime))).append(u".webm");
                        videostoragepath = code.to_utf8(filepath);
                        screen->set_filename(filepath);
                        /*
                         * 设置video
                         * */
                        screen->set_videoWidth(width); //<320~2880>
                        screen->set_videoHeight(height);   //<200~1800>
                        screen->set_videoFPS(fps); //<1~30 fps>
                        screen->set_videoRate(kbps);    // <102~1024kbps>
                        screen->set_videoCodec(VBox::RecordingVideoCodec::AV1);
                        /*
                         * 设置audio
                         * */
                        if (audioquality == 0) {
                            screen->set_audioHz(500);       //不可更改
                            screen->set_audioBits(8);    //8位，16位，24位
                            screen->set_audioChannels(1);    //指定立体声为2，单声道为1

                        } else if (audioquality == 1) {
                            screen->set_audioHz(800);       //不可更改
                            screen->set_audioBits(16);    //8位，16位，24位
                            screen->set_audioChannels(2);    //指定立体声为2，单声道为1

                        } else if (audioquality == 2) {
                            screen->set_audioHz(1000);       //不可更改
                            screen->set_audioBits(24);    //8位，16位，24位
                            screen->set_audioChannels(2);    //指定立体声为2，单声道为1
                        }

                        screen->set_audioCodec(VBox::RecordingAudioCodec::WavPCM);

                        /*
                         * 保存更改
                         * */
                        mutabmachine->saveSettings();
                        session->unlockMachine();
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
                case VBox::MachineState::Saved:
                    callbackdata = " Virtual machine shoud be poweroff!";
                    machine->Release();
                    return false;
            }

        } else {
            callbackdata = " Virtual machine does not exist !";
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
 * 开启屏幕录制
 * 1.虚拟机名字
 * 2.屏幕id
 * 3.打开或关闭(true or false)
 * */
bool vcaVmConfigure::switchRecordingScreen(const string &vmname,
                                           int screenid,
                                           bool on,
                                           string &callbackdata) {
    try {
        vca_toCode code;
        std::u16string name = code.to_utf16(vmname);

        VBox::COMPtr<VBox::ISession> session;
        VBox::COMPtr<VBox::IMachine> machine;
        VBox::COMPtr<VBox::IMachine> mutabmachine;
        VBox::COMPtr<VBox::IRecordingSettings> record;
        VBox::COMPtr<VBox::IRecordingScreenSettings> screen;

        if (!((screenid >= 0) && (screenid <= 7))) {
            callbackdata = "screenid : 0 ~ 7!";
            return false;
        }

        machine = vbox_c->findMachine(name);

        if (machine) {
            switch (machine->state()) {
                case VBox::MachineState::Running:
                case VBox::MachineState::PoweredOff:
                case VBox::MachineState::Starting:
                case VBox::MachineState::Stopping:
                case VBox::MachineState::Saving:
                case VBox::MachineState::Restoring:
                case VBox::MachineState::Saved:
                case VBox::MachineState::Snapshotting: {
                    session = vboxClient_c->session();

                    machine->lockMachine(session, VBox::LockType::Shared);

                    mutabmachine = session->machine();
                    if (mutabmachine) {
                        record = mutabmachine->recordingSettings();
                        /*
                         * 目前支持一次进行一个屏幕的录制
                         * */
                        screen = record->getScreenSettings(static_cast<uint32_t >(screenid));
                        /*
                         * 设置并生效
                         * */
                        record->set_enabled(on);

                        mutabmachine->saveSettings();
                        session->unlockMachine();
                        machine->Release();

                    } else {
                        callbackdata = " Failed to get mutable machine object !";
                        machine->Release();
                        return false;
                    }
                }
                    break;
                case VBox::MachineState::Null:
                case VBox::MachineState::Teleported:
                case VBox::MachineState::Aborted:
                case VBox::MachineState::Paused:
                case VBox::MachineState::Stuck:
                case VBox::MachineState::Teleporting:
                case VBox::MachineState::LiveSnapshotting:
                case VBox::MachineState::TeleportingPausedVM:
                case VBox::MachineState::TeleportingIn:
                case VBox::MachineState::FaultTolerantSyncing:
                case VBox::MachineState::DeletingSnapshotOnline:
                case VBox::MachineState::DeletingSnapshotPaused:
                case VBox::MachineState::OnlineSnapshotting:
                case VBox::MachineState::RestoringSnapshot:
                case VBox::MachineState::DeletingSnapshot:
                case VBox::MachineState::SettingUp: {
                    callbackdata = " Virtual machine should running or starting or poweroff or saved !";
                    machine->Release();
                    return false;
                }

            }

        } else {
            callbackdata = " Virtual machine does not exist !";
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
 * 添加USB口
 * 1.虚拟机名字
 * 2.usb控制器名字
 * 1.usb 类型 EHCI 2.0; XHCI 3.0; OHCI 1.0
 * */
bool vcaVmConfigure::addUSBController(const string &vmname,
                                      const string &usbctlname,
                                      const string &usbType,
                                      string &callbackdata) {
    try {
        vca_toCode code;
        std::u16string name = code.to_utf16(vmname);
        std::u16string usbname = code.to_utf16(usbctlname);
        std::u16string type = code.to_utf16(usbType);

        VBox::COMPtr<VBox::ISession> session;
        VBox::COMPtr<VBox::IMachine> machine;
        VBox::COMPtr<VBox::IMachine> mutabmachine;
        VBox::COMPtr<VBox::IUSBController> ctrl;
//        VBox::COMPtr<VBox::IUSBDevice> device;
//        VBox::COMPtr<VBox::IUSBDeviceFilter> filter;

        machine = vbox_c->findMachine(name);
        if (machine) {
            session = vboxClient_c->session();

            machine->lockMachine(session, VBox::LockType::Shared);

            mutabmachine = session->machine();

            if (strcmp(usbType.c_str(), "XHCI") == 0) {
                /*
                 * usb3.0
                 * */
                ctrl = mutabmachine->addUSBController(usbname, VBox::USBControllerType::XHCI);

            } else if (strcmp(usbType.c_str(), "EHCI") == 0) {
                /*
                 * usb2.0
                 * */
                ctrl = mutabmachine->addUSBController(usbname, VBox::USBControllerType::EHCI);

            } else if (strcmp(usbType.c_str(), "OHCI") == 0) {
                /*
                 * usb1.0
                 * */
//                ctrl = mutabmachine->addUSBController(usbname, VBox::USBControllerType::OHCI);
                callbackdata = "usb 1.0 is default!";
                session->unlockMachine();
                machine->Release();
                return false;

            } else{
                callbackdata = " USB type error !";
                session->unlockMachine();
                machine->Release();
                return false;
            }

            mutabmachine->saveSettings();
            session->unlockMachine();
            machine->Release();
            return true;

        } else {
            callbackdata = " Virtual machine does not exist !";
            machine->Release();
            return false;
        }


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
 * 移除USB控制器
 * 1.虚拟机名字
 * 2.usb控制器名字
 * */
bool vcaVmConfigure::delUSBController(const string &vmname,
                                      const string &usbctlname,
                                      string &callbackdata) {
    try {
        vca_toCode code;
        std::u16string name = code.to_utf16(vmname);
        std::u16string usbname = code.to_utf16(usbctlname);

        VBox::COMPtr<VBox::ISession> session;
        VBox::COMPtr<VBox::IMachine> machine;
        VBox::COMPtr<VBox::IMachine> mutabmachine;
        VBox::COMPtr<VBox::IUSBController> ctrl;
        VBox::COMPtr<VBox::IUSBDevice> device;
        VBox::COMPtr<VBox::IUSBDeviceFilter> filter;

        machine = vbox_c->findMachine(name);
        if (machine) {
            session = vboxClient_c->session();

            machine->lockMachine(session, VBox::LockType::Shared);

            mutabmachine = session->machine();

            mutabmachine->removeUSBController(usbname);

            mutabmachine->saveSettings();
            session->unlockMachine();
            machine->Release();

        } else {
            callbackdata = " Virtual machine does not exist !";
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
 *设置声卡
 * 1.虚拟机名字
 * 3.声音输出
 * 4.声音输入
 * */
bool vcaVmConfigure::setAudio(const string &vmname,
                              bool in,
                              bool out,
                              string &callbackdata) {
    try {
        vca_toCode code;
        std::u16string name = code.to_utf16(vmname);

        VBox::COMPtr<VBox::ISession> session;
        VBox::COMPtr<VBox::IMachine> machine;
        VBox::COMPtr<VBox::IMachine> mutabmachine;
        VBox::COMPtr<VBox::IAudioAdapter> audio;

        machine = vbox_c->findMachine(name);
        if (machine) {
            session = vboxClient_c->session();

            machine->lockMachine(session, VBox::LockType::Shared);

            mutabmachine = session->machine();

            audio = mutabmachine->audioAdapter();
            if(audio){
                audio->set_audioController(VBox::AudioControllerType::AC97);   //声音驱动
                audio->set_audioDriver(VBox::AudioDriverType::ALSA);   //控制芯片

                audio->set_enabledIn(in);
                audio->set_enabledOut(out);

                mutabmachine->saveSettings();
                session->unlockMachine();
                machine->Release();

            } else{
                callbackdata = " No sound card adapter  !";
                session->unlockMachine();
                machine->Release();
                return false;
            }

        } else {
            callbackdata = " Virtual machine does not exist !";
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
 * 声卡开关，传入参数
 * 1.虚拟机名字
 * 2.是否打开
 * */
bool vcaVmConfigure::switchAudio(const string &vmname,
                                 bool enable,
                                 string &callbackdata) {
    try {
        vca_toCode code;
        std::u16string name = code.to_utf16(vmname);

        VBox::COMPtr<VBox::ISession> session;
        VBox::COMPtr<VBox::IMachine> machine;
        VBox::COMPtr<VBox::IMachine> mutabmachine;
        VBox::COMPtr<VBox::IAudioAdapter> audio;

        machine = vbox_c->findMachine(name);
        if (machine) {
            session = vboxClient_c->session();

            machine->lockMachine(session, VBox::LockType::Shared);

            mutabmachine = session->machine();

            audio = mutabmachine->audioAdapter();
            if(audio){
                audio->set_enabled(enable);

                mutabmachine->saveSettings();
                session->unlockMachine();
                machine->Release();

            } else{
                callbackdata = " No sound card adapter !";
                session->unlockMachine();
                machine->Release();
                return false;
            }

        } else {
            callbackdata = " Virtual machine does not exist !";
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
 * 添加串口
 * 1.虚拟机名字
 * 2.slot号，(0~3)
 * */
bool vcaVmConfigure::addSerialPort(const string &vmname,
                                   int slot,
                                   string &callbackdata) {
    try {
        vca_toCode code;
        std::u16string name = code.to_utf16(vmname);

        VBox::COMPtr<VBox::ISession> session;
        VBox::COMPtr<VBox::IMachine> machine;
        VBox::COMPtr<VBox::IMachine> mutabmachine;
        VBox::COMPtr<VBox::ISerialPort> serial;
        if((slot<0)||(slot>4)){
            callbackdata = "serial port: 0 ~ 3 !";
            return false;
        }
        machine = vbox_c->findMachine(name);

        if (machine) {
            session = vboxClient_c->session();

            machine->lockMachine(session, VBox::LockType::Shared);

            mutabmachine = session->machine();

            serial = mutabmachine->getSerialPort(static_cast<uint32_t >(slot));
            if(serial){
                serial->set_enabled(true);

                mutabmachine->saveSettings();
                session->unlockMachine();
                machine->Release();

            } else{
                callbackdata = "No such serial port !";
                session->unlockMachine();
                machine->Release();
                return false;
            }


        } else {
            callbackdata = " Virtual machine does not exist !";
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
 * 移除串口
 * 1.虚拟机名字
 * 2.slot号，(0~3)
 * */
bool vcaVmConfigure::delSerialPort(const string &vmname,
                                   int slot,
                                   string &callbackdata) {
    try {
        vca_toCode code;
        std::u16string name = code.to_utf16(vmname);

        VBox::COMPtr<VBox::ISession> session;
        VBox::COMPtr<VBox::IMachine> machine;
        VBox::COMPtr<VBox::IMachine> mutabmachine;
        VBox::COMPtr<VBox::ISerialPort> serial;
        if((slot<0)||(slot>4)){
            callbackdata = "serial port: 0 ~ 3 !";
            return false;
        }
        machine = vbox_c->findMachine(name);
        if (machine) {
            session = vboxClient_c->session();

            machine->lockMachine(session, VBox::LockType::Shared);

            mutabmachine = session->machine();

            serial = mutabmachine->getSerialPort(static_cast<uint32_t >(slot));
            if(serial){
                serial->set_enabled(false);

                mutabmachine->saveSettings();
                session->unlockMachine();
                machine->Release();

            } else{
                callbackdata = "No such serial port !";
                session->unlockMachine();
                machine->Release();
                return false;
            }


        } else {
            callbackdata = " Virtual machine does not exist !";
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
