# -*-coding:utf-8 -*-

# Copyright(C), 1988-1999, QiTech. Co.,Ltd.
# FileName : cpu_pressure.py
# Author : fanyujia
# Version : 0.10
# Date : 2019/11/12
# Description :
#
# History:
# <author> <time> <version> <desc>

import psutil


# cpu压力（使用率）
def get_cpurate():
    cpu_rate = float(psutil.cpu_percent())
    if cpu_rate >= float(90):
        return dict(code=-1, value='100', obj='cpupress')
    else:
        return dict(code=1, value=str(cpu_rate), obj='cpupress')


if __name__ == '__main__':
    a = get_cpurate()
    print(a)
