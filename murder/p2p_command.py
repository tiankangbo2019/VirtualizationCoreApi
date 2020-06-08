# -*-coding:utf-8 -*-
import errno
import os
import platform
import threading
import psutil

basedir = os.path.abspath(os.path.dirname(__file__))


# 获取网卡名称和其ip地址，不包括回环
def get_netcard():
    netcard_info = []
    info = psutil.net_if_addrs()
    for k, v in info.items():
        for item in v:
            if item[0] == 2 and not item[1] == '127.0.0.1':
                netcard_info.append({k: item[1]})
    return netcard_info


# 开启tracker
def start_tracker():
    # 9998 端口
    OS = platform.system()
    if OS == 'Windows':
        filepath = basedir + '\dist\murder_tracker.py'
    else:
        filepath = basedir + '/dist/murder_tracker.py'

    result = os.system("nohup python {} 1>/dev/null 2>&1 &".format(filepath))
    pid = os.getpid()
    print result, pid
    if pid >= 0:
        return dict(code=1, data="1")
    else:
        return dict(code=-1, data='0')


# 将种子拷贝到其他机器上
def copy_seed(ip, src, dest):
    """
    :param ip: 本机IP地址
    :param src: 目标文件
    :param dest: 目的地
    :return:
    """
    try:
        result = os.system("ansible {} -m synchronize -a 'src={} dest={}'".format(ip, src, dest))
        if result >= 0:
            return True
        else:
            return False

    except Exception as e:
        return False


# 生成种子文件
def produce_seed(file, seedpath, ip, cip, dest):
    """
    :param file:要分享的文件路径
    :param seedpath:生成种子的存储路径
    :param ip:本机ip地址
    :param cip:将种子分享到其他机器的ip地址
    :param dest:其他机器存储种子的路径
    :return:
    """
    OS = platform.system()

    if OS == 'Windows':
        filepath = basedir + '\dist\murder_make_torrent.py'
    else:
        filepath = basedir + '/dist/murder_make_torrent.py'
    result = os.system("python {} {} {}:9998 {}".format(filepath, file, ip, seedpath))
    print result

    if result >= 0:
        ret = copy_seed(cip, seedpath, dest)
        if ret:
            return dict(code=1, data='1')

    else:
        return dict(code=-1, data='0')


# 分发文件
def start_share(file, seedpath, ip):
    """
    :param file: 要分享的文件路径
    :param seedpath: 种子的存储路径
    :param ip:本机的ip地址
    :return:
    """
    try:
        OS = platform.system()
        if OS == 'Windows':
            filepath = basedir + '\dist\murder_client.py'
        else:
            filepath = basedir + '/dist/murder_client.py'
        os.system("nohup python {} seed {} {} {} 1>/dev/null 2>&1 &".format(filepath, seedpath, file, ip))
    except Exception as e:
        print "err-:", e


def distribute_seed(file, seedpath, ip):
    try:
        p = threading.Thread(target=start_share, args=(file, seedpath, ip,))
        p.start()
        data = p.getName()
        if data:
            return dict(code=1, data=data)
        else:
            return dict(code=-1, data='0')
    except Exception as e:
        print "err-:", e


# 下载文件
def download_file(file, seedpath, ip):
    """
    :param file: 文件的下载路径
    :param seedpath: 本机种子的存储路径
    :param ip: 本机的ip地址
    :return:
    """
    OS = platform.system()
    if OS == 'Windows':
        filepath = basedir + '\dist\murder_client.py'
    else:
        filepath = basedir + '/dist/murder_client.py'
    result = os.system("python {} peer {} {} {}".format(filepath, seedpath, file, ip))
    print "result->", result
    if result >= 0:
        return dict(code=1, data='1')
    else:
        return dict(code=-1, data='0')


# 停止进程
def stop_share():
    """
    :param pid: 程序的pid进程
    :return:
    """
    result = os.system("pgrep -f seed|xargs kill -9")
    if result >= 0:
        return dict(code=1, data='1')
    else:
        return dict(code=-1, data='0')


# 停止tracker服务
def stop_tracker():
    result = os.system("pgrep -f murder_tracker.py|xargs kill -9")
    if result >= 0:
        return dict(code=1, data='1')
    else:
        return dict(code=-1, data='0')


if __name__ == '__main__':
    # start_tracker()
    # print get_netcard()
    #produce_seed('E:\PycharmProjects\ubuntu-18.04.3-live-server-amd64.iso', 'E:\PycharmProjects\linux.torrent', '192.168.25.133')
    distribute_seed('/root/CLionProjects/VirtualizationCoreAPI/VirtualizationCoreAPI.zip',
                    '/root/CLionProjects/VirtualizationCoreAPI/VirtualizationCoreAPI.torrent',
                    '192.168.25.110')
    # download_file('E:\PycharmProjects\share\iwant-1.0.14.tar.gz', 'E:\PycharmProjects\iwant.torrent', '192.168.25.133')
