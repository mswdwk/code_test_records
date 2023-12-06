# [python永久更换国内镜像源](/https://zhuanlan.zhihu.com/p/630761244)
# ON Windows 10
```cmd
pip config set global.index-url https://pypi.tuna.tsinghua.edu.cn/simple/
# "Writing to C:\Users\lenovo\AppData\Roaming\pip\pip.ini"
```
# On Linux
```shell
1 # 打开终端 
mkdir /root/.pip

2 # 如果提示文件已存在 直接下一步 貌似安装python后 自己出现的
touch /root/.pip/pip.conf

3 # vim编辑文件
vim pip.conf

4 # 输入下面的内容 这里用的是清华大学的源 也可以换成其他的
[global]
index-url = https://pypi.tuna.tsinghua.edu.cn/simple/

5 # 输入完毕 按esc 接着 冒号+wq 退出
:wq
```
