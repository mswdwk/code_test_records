#coding=utf-8

import math,os
input_file="D:\work\task\lon-lat.txt"
output_file="output.txt"
true=1
false=0
class Point:
    def __init__(self,longitude,latitude):
        self.longitude = longitude #x
        self.latitude = latitude   #y

class Rectangle:
    leftup=Point(0,0)
    right_down=Point(0,0)
    id=0
    in_point_num=0
    def __init__(self,left_up,right_down):
        self.left_up = left_up
        self.right_down = right_down
        
    def in_rect(self,a):
        x_ok = 0
        y_ok = 0
        if self.left_up.longitude <= a.longitude and a.longitude <= self.right_down.longitude:
            x_ok = 1
            
        if self.left_up.latitude >= a.latitude and a.latitude >= self.right_down.latitude:
            y_ok = 1
        if x_ok and y_ok:
            in_point_num+=1
            return true
        return false
        
    def in_rect_list(self,b):
        length=len(b)
        num=0
        for i in range(0,length):
            a=b[i]
            x_ok = 0
            y_ok = 0
            if self.left_up.longitude <= a.longitude and a.longitude <= self.right_down.longitude:
                x_ok = 1

            if self.left_up.latitude >= a.latitude and a.latitude >= self.right_down.latitude:
                y_ok = 1
            if x_ok and y_ok:
                num+=1
            #else:
                #print "point (%f %f) is not in rect"%(a.longitude,a.latitude)
        return num

a=Point(-10,10)
b=Point(10,-10)
c=Point(0,0)

rect = Rectangle(a,b)
'''
rect.id = 123
print rect.id
print rect.left_up.longitude
print rect.left_up.latitude
print rect.in_rect(c)

rects=[]
points=[a,b,c]
for i in range(0,2*2):
    a=Point(-10+i,10)
    b=Point(10+i,-10)
    tmp=Rectangle(a,b)
    rects.append(tmp)
    #rects[i].in_rect(points)
    print i,len(rects),rects[i].left_up.longitude,rects[i].in_rect_list(points)
'''
    
if __name__ == "__main__":
    print "main"
    #inputfd=open(input_file,"r")
    outputfd=open(output_file,"w+")
    line_num=0
    with open(input_file) as f:
        line = f.readline()
        line_num+=1
    print line_num
    
    #inputfd.close()
    outputfd.close()
    