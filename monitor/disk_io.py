# -*-coding:utf-8 -*-
# Copyright(C), 1988-1999, QiTech. Co.,Ltd.
# FileName : disk_io.py
# Author : fanyujia
# Version : 0.10
# Date : 2019/11/12
# Description :
#
# History:
# <author> <time> <version> <desc>

import psutil


# 磁盘的IO读写查询
def disk_ioinfo(disk_name=None, unit='MB'):
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
    if disk_name is None:
        ioInfo = psutil.disk_io_counters()
        read = str(int(ioInfo.read_bytes / divisor))
        write = str(int(ioInfo.write_bytes / divisor))
    else:
        ioInfos = psutil.disk_io_counters(perdisk=True)
        if disk_name in ioInfos.keys():
            read = str(int(ioInfos.get(disk_name).read_bytes / divisor))
            write = str(int(ioInfos.get(disk_name).write_bytes / divisor))
        else:
            return dict(code=-1, read='0', write='0', obj='IO')

    return dict(code=1, read=read, write=write, obj='IO')


if __name__ == '__main__':
    disk = disk_ioinfo()
    print(disk)
