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


# 交换内存的使用率查询
def fun_swap(unit='MB'):
    unit = unit.upper()
    swap_mem = psutil.swap_memory()
    swap_total = swap_mem.total
    swap_used = swap_mem.used
    swap_free = swap_mem.free
    percent = swap_mem.percent
    if float(percent) >= float(100):
        return dict(code=-1, total='0', used='0', free='0', percent='0', obj='swap')
    elif float(percent) >= float(90) and float(percent) < 100:
        return dict(code=-1, total='0', used='0', free='0', percent='0', obj='swap')
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
        return dict(code=1, total=str(float(swap_total / divisor)), used=str(float(swap_used / divisor)),
                    free=str(float(swap_free / divisor)), percent=str(percent), obj='swap')


if __name__ == '__main__':
    a = fun_swap()
    print(a)
