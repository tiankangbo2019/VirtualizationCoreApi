# -*-coding:utf-8 -*-
# Copyright(C), 1988-1999, QiTech. Co.,Ltd.
# FileName : tianma.thrift
# Author : fanyujia
# Version : 0.10
# Date : 2019/11/27
# Description :
#
# History:
# <author> <time> <version> <desc>
import os


def hardware_message():
    '''
    用于获取操作系统的硬件信息
    :return:
    '''

    # 获取cpu模型
    modelmessage = os.popen("cat /proc/cpuinfo | grep 'model name' |uniq").read().split('\n')[0]

    model = modelmessage.split(': ')[-1]

    # 获取osversion
    osversion = os.popen("lsb_release -d").read().split('\t')[1].split('\n')[0]

    # 获取内核信息kernel
    kernel = os.popen("uname -svr").read().split('\n')[0]

    # 获取物理cpu个数
    sockets = os.popen("cat /proc/cpuinfo | grep 'physical id' | sort |uniq | wc -l").read().split('\n')[0]

    # 获取逻辑cpu个数
    cpus = os.popen("cat /proc/cpuinfo| grep 'processor' | wc -l").read().split('\n')[0]


    return dict(code=1, obj='node', cpumodel=model, osversion=osversion, kernel=kernel, sockets=sockets, cpus=cpus)


if __name__ == '__main__':
    result = hardware_message()
    print(result)
