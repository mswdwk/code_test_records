#!/usr/bin/python
# -*- coding: UTF-8 -*-

import numpy as np
import matplotlib.pyplot as plt
import time
 
# 格式化成2016-03-20 11:45:39形式
#print time.strftime("%Y-%m-%d %H:%M:%S", time.localtime()) 

for i in range(0,10):
	ax = plt.subplot(111)
	t = np.arange(0.0, 5.0, 0.01)
	s = np.cos(2*np.pi*t)
	line, = plt.plot(t, s, lw=2)
	plt.annotate(i, xy=(2, 1), xytext=(3, 1.5),arrowprops=dict(facecolor='black', shrink=0.05),)
	plt.ylim(-2,2)
	#plt.show()
	name="plot"+str(i)+".png"
	print "i=",i,",",name
	#plt.text(1, 1, "hell", size=1, ha='center', va='center',color=0x00ff0000)
	plt.text(1.5, 1, i, size=72, ha='center', va='center',color='g')
	current_time=time.strftime("%Y-%m-%d %H:%M:%S", time.localtime()) 
	plt.text(2.5, -1.5, current_time, size=36, ha='center', va='center',color='r')
	plt.savefig(name)
	plt.close()
