import os
from math import *

def angle2radian(angle):
    return 1.0*angle/180.0*pi
def radian2angle(radian):
    return 1.0*radian/pi*180.0

def circle_distance(a_longitude,a_latitude,b_longitude,b_latitude):
    a_longitude=angle2radian(a_longitude)
    b_longitude=angle2radian(b_longitude)
    a_latitude=angle2radian(a_latitude)
    b_latitude=angle2radian(b_latitude)
    res = 0.000000000000+cos(a_latitude)*cos(b_latitude)*( cos(a_longitude)*cos(b_longitude) + sin(a_longitude)*sin(b_longitude) ) + sin(a_latitude)*sin(b_latitude)
    res = 1.0 - res
    if res >= 0.0000000001:
        res=sqrt(2*res)
    else:
        return 0.0
    return res
    
def circle_distance_square(a_longitude,a_latitude,b_longitude,b_latitude):
    a_longitude=angle2radian(a_longitude)
    b_longitude=angle2radian(b_longitude)
    a_latitude=angle2radian(a_latitude)
    b_latitude=angle2radian(b_latitude)
    res = 0.000000000000+cos(a_latitude)*cos(b_latitude)*( cos(a_longitude)*cos(b_longitude) + sin(a_longitude)*sin(b_longitude) ) + sin(a_latitude)*sin(b_latitude)
    res = 1.0 - res
    return 2.0*res

beijing=(116.407395,39.904211)
nanjing=(118.796877,32.060255)

#qiuxinjiao= circle_distance(116.407395,39.904211,118.796877,32.060255)
#qiuxinjiao= circle_distance(beijing[0],beijing[1],nanjing[0],nanjing[1])
#earth_radius=6371.00
#print "%d km "%(qiuxinjiao*earth_radius)
#           A
#      c  /   \  b
#        /     \
#      B ------ C
#           a
def three_point_angle(A,B,C):
    cc=circle_distance_square(A[0],A[1],B[0],B[1])
    bb=circle_distance_square(A[0],A[1],C[0],C[1])
    aa=circle_distance_square(C[0],C[1],B[0],B[1])
    radian_A=acos((bb+cc-aa)*1.0/2.0/sqrt(bb*cc*1.0))
    angle_A = radian2angle(radian_A)
    return angle_A

A=beijing
B=(110.0,32.0)
C=nanjing

print three_point_angle(A,B,C)
