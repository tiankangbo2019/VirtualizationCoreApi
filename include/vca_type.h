//
// Created by root on 2019/11/20.
//

#ifndef VIRTUALIZATIONCOREAPI_VCA_TYPE_H
#define VIRTUALIZATIONCOREAPI_VCA_TYPE_H

typedef signed char int8_t;

typedef short int int16_t;

typedef int int32_t;

# if __WORDSIZE == 64
typedef long int int64_t;
# else
__extension__
typedef long long int     int64_t;
#endif

namespace VCA {
    enum class MachineState : int32_t {
        Null = 0,
        PoweredOff = 1,     //关机
        Saved = 2,          //saved状态下虚拟机休眠, 保存在磁盘中
        Teleported = 3,
        Aborted = 4,    //故障，此时虚拟机可能是由于不当操作，导致了故障，可以尝试调用开机进行自动恢复
        Running = 5,    //正在运行
        Paused = 6,     //挂起状态, 可调用恢复运行接口进行恢复
        Stuck = 7,
        Teleporting = 8,
        LiveSnapshotting = 9,
        Starting = 10,      //正在开机，此时状态应避免其他操作
        Stopping = 11,      //关机中，此时状态应避免其他操作
        Saving = 12,        //保存状态中，此时应避免其他操作
        Restoring = 13,     //由saved状态开机时候，此状态下应避免其他操作
        TeleportingPausedVM = 14,
        TeleportingIn = 15,
        FaultTolerantSyncing = 16,
        DeletingSnapshotOnline = 17,
        DeletingSnapshotPaused = 18,
        OnlineSnapshotting = 19,
        RestoringSnapshot = 20,     //恢复快照中，此时应避免其他操作
        DeletingSnapshot = 21,      //删除快照中，此时应该避免其他操作
        SettingUp = 22,
        Snapshotting = 23,      //创建快照中，此时应避免其他操作
        FirstOnline = 5,
        LastOnline = 19,
        FirstTransient = 8,
        LastTransient = 23,
    };

    enum class LockType : int32_t {
        Null = 0,
        Shared = 1,
        Write = 2,
        VM = 3,
    };

    enum class DeviceActivity : int32_t {
        Null = 0,
        Idle = 1,
        Reading = 2,
        Writing = 3,
    };

    enum class DeviceType : int32_t {
        Null = 0,
        Floppy = 1,
        DVD = 2,
        HardDisk = 3,
        Network = 4,
        USB = 5,
        SharedFolder = 6,
        Graphics3D = 7,
    };

    enum class MediumState {
        NotCreated = 0,
        Created = 1,
        LockedRead = 2,
        LockedWrite = 3,
        Inaccessible = 4,
        Creating = 5,
        Deleting = 6,
    };

    enum class MediumType {
        Normal = 0,
        Immutable = 1,
        Writethrough = 2,
        Shareable = 3,
        Readonly = 4,
        MultiAttach = 5,
    };

    enum class SessionState {
        Null = 0,
        Unlocked = 1,
        Locked = 2,
        Spawning = 3,
        Unlocking = 4,
    };

    enum class SessionType {
        Null = 0,
        WriteLock = 1,
        Remote = 2,
        Shared = 3,
    };
    enum class StorageBus {
        Null = 0,
        IDE = 1,
        SATA = 2,
        SCSI = 3,
        Floppy = 4,
        SAS = 5,
        USB = 6,
        PCIe = 7,
    };
    enum class CleanupMode {
        UnregisterOnly = 1,
        DetachAllReturnNone = 2,
        DetachAllReturnHardDisksOnly = 3,
        Full = 4,
    };
    enum class VirtualSystemDescriptionType {
        Ignore = 1,
        OS = 2,
        Name = 3,
        Product = 4,
        Vendor = 5,
        Version = 6,
        ProductUrl = 7,
        VendorUrl = 8,
        Description = 9,
        License = 10,
        Miscellaneous = 11,
        CPU = 12,
        Memory = 13,
        HardDiskControllerIDE = 14,
        HardDiskControllerSATA = 15,
        HardDiskControllerSCSI = 16,
        HardDiskControllerSAS = 17,
        HardDiskImage = 18,
        Floppy = 19,
        CDROM = 20,
        NetworkAdapter = 21,
        USBController = 22,
        SoundCard = 23,
        SettingsFile = 24,
        BaseFolder = 25,
        PrimaryGroup = 26,
        CloudInstanceShape = 27,
        CloudDomain = 28,
        CloudBootDiskSize = 29,
        CloudBucket = 30,
        CloudOCIVCN = 31,
        CloudPublicIP = 32,
        CloudProfileName = 33,
        CloudOCISubnet = 34,
        CloudKeepObject = 35,
        CloudLaunchInstance = 36,
    };
    enum class GraphicsControllerType {
        Null = 0,
        VBoxVGA = 1,
        VMSVGA = 2,
        VBoxSVGA = 3,
    };

    enum class RecordingAudioCodec
    {
        None = 0,
        WavPCM = 1,
        Opus = 2,
    };

    enum class RecordingDestination
    {
        None = 0,
        File = 1,
    };

    enum class RecordingFeature
    {
        None = 0,
        Video = 1,
        Audio = 2,
    };

    enum class RecordingVideoCodec
    {
        None = 0,
        VP8 = 1,
        VP9 = 2,
        AV1 = 3,
    };

    enum class RecordingVideoRateControlMode
    {
        CBR = 0,
        VBR = 1,
    };

    enum class RecordingVideoScalingMethod
    {
        None = 0,
        NearestNeighbor = 1,
        Bilinear = 2,
        Bicubic = 3,
    };

    enum class HWVirtExPropertyType
    {
        Null = 0,
        Enabled = 1,
        VPID = 2,
        NestedPaging = 3,
        UnrestrictedExecution = 4,
        LargePages = 5,
        Force = 6,
        UseNativeApi = 7,
    };
    enum class AudioControllerType
    {
        AC97 = 0,
        SB16 = 1,
        HDA = 2,
    };

    enum class AudioDriverType
    {
        Null = 0,
        WinMM = 1,
        OSS = 2,
        ALSA = 3,
        DirectSound = 4,
        CoreAudio = 5,
        MMPM = 6,
        Pulse = 7,
        SolAudio = 8,
    };
    enum class VBoxEventType
    {
        Invalid = 0,
        Any = 1,
        Vetoable = 2,
        MachineEvent = 3,
        SnapshotEvent = 4,
        InputEvent = 5,
        LastWildcard = 31,
        OnMachineStateChanged = 32,
        OnMachineDataChanged = 33,
        OnExtraDataChanged = 34,
        OnExtraDataCanChange = 35,
        OnMediumRegistered = 36,
        OnMachineRegistered = 37,
        OnSessionStateChanged = 38,
        OnSnapshotTaken = 39,
        OnSnapshotDeleted = 40,
        OnSnapshotChanged = 41,
        OnGuestPropertyChanged = 42,
        OnMousePointerShapeChanged = 43,
        OnMouseCapabilityChanged = 44,
        OnKeyboardLedsChanged = 45,
        OnStateChanged = 46,
        OnAdditionsStateChanged = 47,
        OnNetworkAdapterChanged = 48,
        OnSerialPortChanged = 49,
        OnParallelPortChanged = 50,
        OnStorageControllerChanged = 51,
        OnMediumChanged = 52,
        OnVRDEServerChanged = 53,
        OnUSBControllerChanged = 54,
        OnUSBDeviceStateChanged = 55,
        OnSharedFolderChanged = 56,
        OnRuntimeError = 57,
        OnCanShowWindow = 58,
        OnShowWindow = 59,
        OnCPUChanged = 60,
        OnVRDEServerInfoChanged = 61,
        OnEventSourceChanged = 62,
        OnCPUExecutionCapChanged = 63,
        OnGuestKeyboard = 64,
        OnGuestMouse = 65,
        OnNATRedirect = 66,
        OnHostPCIDevicePlug = 67,
        OnVBoxSVCAvailabilityChanged = 68,
        OnBandwidthGroupChanged = 69,
        OnGuestMonitorChanged = 70,
        OnStorageDeviceChanged = 71,
        OnClipboardModeChanged = 72,
        OnDnDModeChanged = 73,
        OnNATNetworkChanged = 74,
        OnNATNetworkStartStop = 75,
        OnNATNetworkAlter = 76,
        OnNATNetworkCreationDeletion = 77,
        OnNATNetworkSetting = 78,
        OnNATNetworkPortForward = 79,
        OnGuestSessionStateChanged = 80,
        OnGuestSessionRegistered = 81,
        OnGuestProcessRegistered = 82,
        OnGuestProcessStateChanged = 83,
        OnGuestProcessInputNotify = 84,
        OnGuestProcessOutput = 85,
        OnGuestFileRegistered = 86,
        OnGuestFileStateChanged = 87,
        OnGuestFileOffsetChanged = 88,
        OnGuestFileRead = 89,
        OnGuestFileWrite = 90,
        OnRecordingChanged = 91,
        OnGuestUserStateChanged = 92,
        OnGuestMultiTouch = 93,
        OnHostNameResolutionConfigurationChange = 94,
        OnSnapshotRestored = 95,
        OnMediumConfigChanged = 96,
        OnAudioAdapterChanged = 97,
        OnProgressPercentageChanged = 98,
        OnProgressTaskCompleted = 99,
        OnCursorPositionChanged = 100,
        Last = 101,
    };
}

const struct OSTypeId {
    std::u16string Other = u"Other";
    std::u16string Other_64 = u"Other_64";
    std::u16string Windows31 = u"Windows31";
    std::u16string Windows95 = u"Windows95";
    std::u16string Windows98 = u"Windows98";
    std::u16string WindowsMe = u"WindowsMe";
    std::u16string WindowsNT3x = u"WindowsNT3x";
    std::u16string WindowsNT4 = u"WindowsNT4";
    std::u16string Windows2000 = u"Windows2000";
    std::u16string WindowsXP = u"WindowsXP";
    std::u16string WindowsXP_64 = u"WindowsXP_64";
    std::u16string Windows2003 = u"Windows2003";
    std::u16string Windows2003_64 = u"Windows2003_64";
    std::u16string WindowsVista = u"WindowsVista";
    std::u16string WindowsVista_64 = u"WindowsVista_64";
    std::u16string Windows2008 = u"Windows2008";
    std::u16string Windows2008_64 = u"Windows2008_64";
    std::u16string Windows7 = u"Windows7";
    std::u16string Windows7_64 = u"Windows7_64";
    std::u16string Windows8 = u"Windows8";
    std::u16string Windows8_64 = u"Windows8_64";
    std::u16string Windows81 = u"Windows81";
    std::u16string Windows81_64 = u"Windows81_64";
    std::u16string Windows2012_64 = u"Windows2012_64";
    std::u16string Windows10 = u"Windows10";
    std::u16string Windows10_64 = u"Windows10_64";
    std::u16string Windows2016_64 = u"Windows2016_64";
    std::u16string Windows2019_64 = u"Windows2019_64";
    std::u16string WindowsNT = u"WindowsNT";
    std::u16string WindowsNT_64 = u"WindowsNT_64";
    std::u16string Linux22 = u"Linux22";
    std::u16string Linux24 = u"Linux24";
    std::u16string Linux24_64 = u"Linux24_64";
    std::u16string Linux26 = u"Linux26";
    std::u16string Linux26_64 = u"Linux26_64";
    std::u16string ArchLinux = u"ArchLinux";
    std::u16string ArchLinux_64 = u"ArchLinux_64";
    std::u16string Debian = u"Debian";
    std::u16string Debian_64 = u"Debian_64";
    std::u16string Fedora = u"Fedora";
    std::u16string Fedora_64 = u"Fedora_64";
    std::u16string Gentoo = u"Gentoo";
    std::u16string Gentoo_64 = u"Gentoo_64";
    std::u16string Mandriva = u"Mandriva";
    std::u16string Mandriva_64 = u"Mandriva_64";
    std::u16string Oracle = u"Oracle";
    std::u16string Oracle_64 = u"Oracle_64";
    std::u16string RedHat = u"RedHat";
    std::u16string RedHat_64 = u"RedHat_64";
    std::u16string OpenSUSE = u"OpenSUSE";
    std::u16string OpenSUSE_64 = u"OpenSUSE_64";
    std::u16string Turbolinux = u"Turbolinux";
    std::u16string Turbolinux_64 = u"Turbolinux_64";
    std::u16string Ubuntu = u"Ubuntu";
    std::u16string Ubuntu_64 = u"Ubuntu_64";
    std::u16string Xandros = u"Xandros";
    std::u16string Xandros_64 = u"Xandros_64";
    std::u16string Linux = u"Linux";
    std::u16string Linux_64 = u"Linux_64";

    std::u16string Solaris = u"Solaris";
    std::u16string Solaris_64 = u"Solaris_64";
    std::u16string OpenSolaris = u"OpenSolaris";
    std::u16string OpenSolaris_64 = u"OpenSolaris_64";
    std::u16string Solaris11_64 = u"Solaris11_64";
    std::u16string FreeBSD = u"FreeBSD";
    std::u16string FreeBSD_64 = u"FreeBSD_64";
    std::u16string OpenBSD = u"OpenBSD";
    std::u16string OpenBSD_64 = u"OpenBSD_64";
    std::u16string NetBSD = u"NetBSD";
    std::u16string NetBSD_64 = u"NetBSD_64";
    std::u16string OS2Warp3 = u"OS2Warp3";
    std::u16string OS2Warp4 = u"OS2Warp4";
    std::u16string OS2Warp45 = u"OS2Warp45";
    std::u16string OS2eCS = u"OS2eCS";
    std::u16string OS21x = u"OS21x";
    std::u16string OS2 = u"OS2";

    std::u16string MacOS = u"MacOS";
    std::u16string MacOS_64 = u"MacOS_64";
    std::u16string MacOS106 = u"MacOS106";
    std::u16string MacOS106_64 = u"MacOS106_64";
    std::u16string MacOS107_64 = u"MacOS107_64";
    std::u16string MacOS108_64 = u"MacOS108_64";
    std::u16string MacOS109_64 = u"MacOS109_64";
    std::u16string MacOS1010_64 = u"MacOS1010_64";
    std::u16string MacOS1011_64 = u"MacOS1011_64";
    std::u16string MacOS1012_64 = u"MacOS1012_64";
    std::u16string MacOS1013_64 = u"MacOS1013_64";

    std::u16string DOS = u"DOS";
    std::u16string Netware = u"Netware";
    std::u16string L4 = u"L4";
    std::u16string QNX = u"QNX";
    std::u16string JRockitVE = u"JRockitVE";
    std::u16string VBoxBS_64 = u"VBoxBS_64";

} ostype;

const struct storageBus {
    std::u16string ide = u"IDE";
    std::u16string sata = u"SATA";
    std::u16string scsi = u"SCSI";
    std::u16string floppy = u"Floppy";
    std::u16string sas = u"SAS";
    std::u16string usb = u"USB";
    std::u16string pcie = u"PCIe";

} storagebus;

const struct diskType {
    std::u16string vdi = u"VDI";
    std::u16string vhd = u"VHD";
    std::u16string vmdk = u"VMDK";

} disktype;

const struct GraphicsControllerType {

    std::string vboxVGA = "VBoxVGA";
    std::string vmsVGA = "VMSVGA";
    std::string vboxSVGA = "VBoxSVGA";

} graphicsCtrlType;

#endif //VIRTUALIZATIONCOREAPI_VCA_TYPE_H
