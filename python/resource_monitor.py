#!/usr/bin/python
# -*- coding: UTF-8 -*-
#author: Ensys Yule
#date: 2019/12/21
#mail: 609961022@qq.com
import libvirt
import time
from xml.etree import ElementTree
import os
import commands
import sys
'''将bytes数转换成更加直观的符号显示'''
def bytes2symbols(bytes_value):
 
    '''单位符号'''
    symbols = ('K' ,'M', 'G', 'T', 'P', 'E', 'Z', 'Y')
 
    '''每个单位对应的bytes数的字典,先定义为空'''
    prefix={}
 
    '''要得到这样的{K:1024, M:1024*1024,G:1024**3},进行for循环'''
    for i,s in enumerate(symbols):
        # '''enumerate()为列举函数'''
        # '''i:代表下标'''
        # '''s:代表改下标对应的值'''
 
        # '''取到符号元组的值,作为prfix字典的key,根据key给value进行赋值'''
        prefix[s]=1024**(i+1)
 
    # '''打印得到的对应字典'''
    # print(prefix)
    symbols_value=0
    symbol=''
    # '''循环prefix字典,得到转换值'''
    for key,value in prefix.items():
        if bytes_value >=value :
            symbols_value=bytes_value/value
            symbol=key
        # 如果不满足最小的KB,则以B显示
        elif bytes_value <1024:
            return  '%0.2fB'%bytes_value
        # '''返回转换值(str)'''
    return  '%0.2f%sB'%(symbols_value,symbol)

while True: 
    os.system('clear')
    conn = libvirt.open("qemu:///system")
    print '''
*********************************************
                   _ooOoo_
                  o8888888o
                  88" . "88
                  (| -_- |)
                  O\  =  /O
               ____/`---'\____
             .'  \\|     |//  `.
            /  \\|||  :  |||//  \
           /  _||||| -:- |||||-  \
           |   | \\\  -  /// |   |
           | \_|  ''\---/''  |   |
           \  .-\__  `-`  ___/-. /
         ___`. .'  /--.--\  `. . __
      ."" '<  `.___\_<|>_/___.'  >'"".
     | | :  `- \`.;`\ _ /`;.`/ - ` : | |
     \  \ `-.   \_ __\ /__ _/   .-` /  /
======`-.____`-.___\_____/___.-`____.-'======
                   `=---='
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
           佛祖保佑       永不宕机'''
    print '\033[0;37;41m%s\033[0m'%'\n\n######################     实时监控kvm虚拟机信息--CPU,内存，磁盘I/O    ##################'
    print '\n作者：黄宇轩'
    print '时间: 2019/12/21'
    print 'Ctrl+C 可退出程序,脚本每6秒执行一次'
    if len(conn.listDomainsID()) <= 0 :
        print '\033[0;37;41m%s\033[0m'%'没有正在运行的虚拟机，程序退出.'
        os.system('command')
        time.sleep(1)
        sys.exit()
    try:
        for id in reversed(conn.listDomainsID()):
            print '\n'
            domain = conn.lookupByID(id)
            print '实例名：'+'\033[0;37;44m%s\033[0m'%domain.name() + '\n'
            print '状态：'+'\033[0;37;42m%s\033[0m'%'开机' if domain.info()[0]==1 else '\033[0;37;41m%s\033[0m'%'关机',
            print '最大内存：' + (str)(domain.info()[1]/1024) + 'Mb',
            print 'CPU数量：' + (str)(domain.info()[3]),
            print 'CPU时间：' + (str)(domain.info()[4])

            #开始计算虚拟机内存
            domain.setMemoryStatsPeriod(10)
            meminfo = domain.memoryStats()
            free_mem = float(meminfo['unused'])
            total_mem = float(meminfo['available'])
            util_mem = ((total_mem-free_mem) / total_mem)*100
            print ('内存已使用：' + str(util_mem)) + '%'

            #开始计算磁盘I/O
            tree = ElementTree.fromstring(domain.XMLDesc())
            devices = tree.findall('devices/disk/target')
            for d in devices:
                device = d.get('dev')
                try:
                    devstats = domain.blockStats(device)
                    print '磁盘设备：'+(device,devstats)[0],
                    read_bytes = bytes2symbols((device,devstats)[1][0])
                    write_bytes = bytes2symbols((device,devstats)[1][2])

                    print('读:%10s'%read_bytes),
                    print '读取请求：' + (str)((device,devstats)[1][1])+ '次',
                    print('写:%10s'%read_bytes),
                    print '写入请求：' + (str)((device,devstats)[1][3])+ '次'
                except libvirt.libvirtError:
                    pass
            
            #开始计算cpu使用率
            t1 = time.time()
            c1 = int (domain.info()[4])
            time.sleep(0.05);
            t2 = time.time();
            c2 = int (domain.info()[4])
            c_nums = int (domain.info()[3])
            usage = (c2-c1)*100/((t2-t1)*c_nums*1e9)
            print ("CPU 已使用：" + (str)(usage)) + '%'
    except: 
        pass
    os.system('command')
    time.sleep(6)

