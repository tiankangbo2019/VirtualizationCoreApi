# coding:utf8
import psutil
import datetime


def uptime():

    _up = {}
    _up['code'] = 1
    _up['obj'] = 'uptime'

    try:
        _up['value'] = datetime.datetime.fromtimestamp(psutil.boot_time()).strftime("%Y-%m-%d %H: %M: %S")
        return _up

    except:
        _up['code'] = -1
        _up['value'] = '0'
        return _up


if __name__ == '__main__':
    print(uptime())