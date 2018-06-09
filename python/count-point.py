#coding=utf-8

import math,os
input_file="lon-lat.txt"
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
        #print "length = ",length,self.left_up,self.right_down
        num=0
        for i in range(0,length):
            a=b[i]
            x_ok = 0
            y_ok = 0
            #print a
            #if self.left_up.longitude <= a.longitude and a.longitude <= self.right_down.longitude:
            if float(self.left_up[0]) <= float(a[0]) and float(a[0]) <= float(self.right_down[0]):
                x_ok = 1
            #if self.left_up.latitude >= a.latitude and a.latitude >= self.right_down.latitude:
            if float(self.left_up[1]) >= float(a[1]) and float(a[1]) >= float(self.right_down[1]):
                y_ok = 1
            if x_ok and y_ok:
                num+=1
            #else:
                #print "point (%f %f) is not in rect"%(a.longitude,a.latitude)
                #print "point (%f %f) is not in rect"%(float(a[0]),float(a[1]))
        return num

a=Point(-10,10)
b=Point(10,-10)
c=Point(0,0)

rect = Rectangle(a,b)
                           #y     #x
#phone1 	phone2 	id 	latitude	longitude	address	date

def find_left_up_right_down_point(points):
    num=len(points)
    # 0 longitude  , 1 latitude
    left_up=list(points[0])
    right_down=list(points[0])
    for i in range(0,num):
        if left_up[0] > points[i][0] :
            left_up[0] = points[i][0]
            
        if left_up[1] < points[i][1] :
            left_up[1] = points[i][1]
            
        if right_down[0] < points[i][0] :
            right_down[0] = points[i][0]
            
        if right_down[1] > points[i][1] :
            right_down[1] = points[i][1]
        
    return left_up,right_down
    
def find_point_num(points,longth_split_num,width_split_num):
    left_up,right_down = find_left_up_right_down_point(points)
    if longth_split_num < 1.0 or width_split_num < 1.0:
        return 0.0
    longth_each = (float(right_down[0]) - float(left_up[0]))*1.0/longth_split_num
    width_each = (float(left_up[1]) - float(right_down[1]))*1.0/width_split_num
    sub_rect_id = 0
    length = len(points)
    total_in_point_num=0
    in_point_num=[]
    for i in range(0,longth_split_num):
        for j in range(0,width_split_num):
            sub_rect_left_up=[0,0]
            sub_rect_right_down=[0,0]
            sub_rect_left_up[0] = float(left_up[0]) + longth_each*i*1.0
            sub_rect_left_up[1] = float(left_up[1]) - width_each*j*1.0
            sub_rect_right_down[0] = float(left_up[0]) + longth_each*(i + 1)*1.0
            sub_rect_right_down[1] = float(left_up[1]) - width_each *(j + 1)*1.0
            sub_rect = Rectangle(tuple(sub_rect_left_up),tuple(sub_rect_right_down))
            in_point_num.append ( sub_rect.in_rect_list(points) )
            print sub_rect_id,sub_rect_left_up,sub_rect_right_down,in_point_num[sub_rect_id]
            total_in_point_num += in_point_num[sub_rect_id]
            sub_rect_id += 1
    
    print "length ",length,"total in point num ",total_in_point_num

data=[]
points_global=[]
if __name__ == "__main__":
    #print "main"
    #inputfd=open(input_file,"r")
    outputfd=open(output_file,"w+")
    line_num=0
    with open(input_file) as f:
        line = f.readline()
        line_id = 0
        while line:
            line = f.readline()
            line_num+=1
            #ph1,ph2,id,x,y,addr,date
            tmp=line.split()
            line_id+=1
            if len(tmp) < 4:
                #print line_id,tmp
                continue
            tup1=(tmp[4],tmp[3])
            #print tup1
            #print len(longitude_latitude),tmp[4],tmp[3]
            points_global.append(tup1)
            data.append(tuple(tmp))
            
    left_up,right_down = find_left_up_right_down_point(points_global)
    print "Rectangle which contains all given points is ",left_up,right_down
    print >> outputfd, left_up,right_down 
    #case 4*4
    longth_split_num = 4
    width_split_num = 4
    find_point_num(points_global,longth_split_num,width_split_num)
    
    #inputfd.close()
    outputfd.close()
    