apt-get install lm-sensors sensors-applet
pip install -i https://pypi.tuna.tsinghua.edu.cn/simple netifaces psutils
apt install lm-sensors

apt install zlib1g-dev   //grpc
apt install gawk

apt-get install libjsoncpp-dev libjsoncpp1

清华：https://pypi.tuna.tsinghua.edu.cn/simple
阿里云：http://mirrors.aliyun.com/pypi/simple/
中国科技大学 https://pypi.mirrors.ustc.edu.cn/simple/
华中理工大学：http://pypi.hustunique.com/
山东理工大学：http://pypi.sdutlinux.org/
豆瓣：http://pypi.douban.com/simple/
note：新版ubuntu要求使用https源，要注意。


可以在使用pip的时候加参数-i https://pypi.tuna.tsinghua.edu.cn/simple
例如：pip install -i https://pypi.tuna.tsinghua.edu.cn/simple pyspider，这样就会从清华这边的镜像去安装pyspider库。
