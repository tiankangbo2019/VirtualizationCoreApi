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


# 磁盘使用率查询
def fun_disk(unit='MB', disk_path='/var/lib/vcloud'):
    unit = unit.upper()
    try:
        disk_mem = psutil.disk_usage(disk_path)
    except:
        return dict(code=-2, total='0', used='0', free='0', percent='0', obj='disk')

    disk_total = disk_mem.total
    disk_used = disk_mem.used
    disk_free = disk_mem.free
    percent = disk_mem.percent
    if float(percent) >= float(90) and float(percent) < 100:
        return dict(code=-1, total='0', used='0', free='0', percent='0', obj='disk')
    else:
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
        return dict(code=1, total=str(float(disk_total / divisor)), used=str(float(disk_used / divisor)),
                    free=str(float(disk_free / divisor)), percent=str(percent), obj='disk')


if __name__ == '__main__':
    rate = fun_disk("MB", "/")
    print(rate)
