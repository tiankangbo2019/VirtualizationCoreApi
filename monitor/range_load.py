# -*-coding:utf-8 -*-
# Copyright(C), 1988-1999, QiTech. Co.,Ltd.
# FileName : tianma.thrift
# Author : fanyujia
# Version : 0.10
# Date : 2019/11/12
# Description :
#
# History:
# <author> <time> <version> <desc>


def get_load():

    loadavg = {}
    loadavg['code'] = 1
    loadavg['obj'] = 'load'

    try:
        f = open("/proc/loadavg")
        con = f.read().split()
        f.close()

        loadavg['lavg_1'] = con[0]
        loadavg['lavg_5'] = con[1]
        loadavg['lavg_15'] = con[2]

        return loadavg

    except:
        loadavg['code'] = -1
        loadavg['lavg_1'] = '0'
        loadavg['lavg_5'] = '0'
        loadavg['lavg_15'] = '0'

        return loadavg


if __name__ == '__main__':
    load = get_load()
    print(load)
