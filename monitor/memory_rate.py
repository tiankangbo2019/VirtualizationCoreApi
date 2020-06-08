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


# 内存使用率查询
def fun_memory(unit='MB'):
    unit = unit.upper()
    mem = psutil.virtual_memory()
    mem_total = mem.total
    mem_used = mem.used
    mem_free = mem.free
    percent = mem.percent
    if float(percent) >= float(100):
        return dict(code=-1, total='0', used='0', free='0', percent='0', obj='memory')
    elif float(percent) >= float(90) and float(percent) < 100:
        return dict(code=-1, total='0', used='0', free='0', percent='0', obj='memory')
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
        return dict(code=1, total=str(float(mem_total / divisor)), used=str(float(mem_used / divisor)),
                    free=str(float(mem_free / divisor)), percent=str(percent), obj='memory')


if __name__ == '__main__':
    memory = fun_memory('MB')
    print(memory)
