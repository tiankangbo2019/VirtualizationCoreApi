# -*-coding:utf-8 -*-

# Copyright(C), 1988-1999, QiTech. Co.,Ltd.
# FileName : cpu_temperature.py
# Author : fanyujia
# Version : 0.10
# Date : 2019/11/12
# Description :
#
# History:
# <author> <time> <version> <desc>
import os


def get_cpuT():
    try:
        cpu_Ts = os.popen("sensors | egrep '(Core)'|awk '{print $3}' | cut -c2-5").read()
        cpu_Ts = cpu_Ts.split('\n')
    except OSError as e:
        return dict(code=-1, value='100', obj='cputemp')

    cpu_Ts.remove('')
    for cpu_T in cpu_Ts:
        if float(cpu_T) >= float(100.0):
            return dict(code=-1, value='100', obj='cputemp')
        elif float(cpu_T) >= float(90.0) and float(cpu_T) < float(100.0):
            return dict(code=-1, value='90', obj='cputemp')
        elif float(cpu_T) >= float(80.0) and float(cpu_T) < float(90.0):
            return dict(code=-1, value='80', obj='cputemp')
        else:
            pass

    return dict(code=1, value=max(cpu_Ts), obj='cputemp')


if __name__ == '__main__':
    temperature = get_cpuT()
    print(temperature)
