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

import netifaces


def net_count():
    netnames = netifaces.interfaces()
    data = {
        'code': '1',
        'obj': 'netcount'
    }
    if not netnames:
        return {'code': '-1', 'msg': '0'}
    for netname in netnames:
        try:
            address = netifaces.ifaddresses(netname)[netifaces.AF_INET][0]['addr']
        except:
            address = '0'
        data[netname] = address
    return data


if __name__ == '__main__':
    count = net_count()
    print(count)
