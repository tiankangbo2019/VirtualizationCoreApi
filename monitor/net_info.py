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

import psutil


# 网卡的上下行流量
def net_info(net_name=None, unit='MB'):
    unit = unit.upper()
    if unit == 'B':
        divisor = 1
    elif unit == 'KB':
        divisor = 1024
    elif unit == 'MB':
        divisor = 1024 ** 2
    elif unit == 'GB':
        divisor = 1024 ** 3
    elif unit == 'TB':
        divisor = 1024 ** 4
    elif unit == 'PB':
        divisor = 1024 ** 5
    else:
        divisor = 1024 ** 6
    if net_name is None:
        netinfo = psutil.net_io_counters()
        send = str(float(netinfo.bytes_sent / divisor))
        recv = str(float(netinfo.bytes_recv / divisor))
    else:
        netinfos = psutil.net_io_counters(pernic=True)
        if net_name in netinfos.keys():
            send = str(float(netinfos.get(net_name).bytes_sent) / divisor)
            recv = str(float(netinfos.get(net_name).bytes_recv) / divisor)
        else:
            return dict(code=-1, send='0', recv='0', obj='netinfo')
    return dict(code=1, send=send, recv=recv, obj='netinfo')


if __name__ == '__main__':
    ninfo = net_info()
    print(ninfo)
