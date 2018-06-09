import os
from math import *
a="abc\t123\tc"
print tuple(a.split())
'''
def getAngle( lon1,  lat1,  lon2, lat2):
        fLat = Math.pi * (lat1) / 180.0
        fLng = Math.pi * (lon1) / 180.0
        tLat = math.pi * (lat2) / 180.0
        tLng = math.pi * (lon2) / 180.0
  
        degree = (math.atan2(math.sin(tLng - fLng) * math.cos(tLat), math.cos(fLat) * math.sin(tLat) - math.sin(fLat) * math.cos(tLat) * math.cos(tLng - fLng))) * 180.0 / math.pi
        if (degree >= 0) :
            return degree
        else :
            return 360 + degree
            
print getAngle(0,0,45,45)
'''
#print asin(0.5)*180.0/pi
#jiaodu2hudu
class Point:
    def __init__(self,longitude,latitude):
        self.longitude = longitude #x
        self.latitude = latitude   #y
def angle2radian(angle):
    return 1.0*angle/180.0*pi
def radian2angle(radian):
    return 1.0*radian/pi*180.0

# a is  beijing  , b is nanjing

def circle_distance(a_longitude,a_latitude,b_longitude,b_latitude):
    a_longitude=angle2radian(a_longitude)
    b_longitude=angle2radian(b_longitude)
    a_latitude=angle2radian(a_latitude)
    b_latitude=angle2radian(b_latitude)
    res = 0.000000000000+cos(a_latitude)*cos(b_latitude)*( cos(a_longitude)*cos(b_longitude) + sin(a_longitude)*sin(b_longitude) ) + sin(a_latitude)*sin(b_latitude)
    #print res
    res = 1.0 - res
    #res=0.0000000001
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
    #print res
    res = 1.0 - res
    #res=0.0000000001
    return 2.0*res
    
#print sin(angle2radian(30))
beijing=(116.407395,39.904211)
nanjing=(118.796877,32.060255)

#qiuxinjiao= circle_distance(116.407395,39.904211,118.796877,32.060255)
qiuxinjiao= circle_distance(beijing[0],beijing[1],nanjing[0],nanjing[1])
earth_radius=6371.00
#print "%d km "%(qiuxinjiao*earth_radius)
def three_point_angle(A,B,C):
    cc=circle_distance_square(A[0],A[1],B[0],B[1])
    bb=circle_distance_square(A[0],A[1],C[0],C[1])
    aa=circle_distance_square(C[0],C[1],B[0],B[1])
    angle_A=acos((bb+cc-aa)*1.0/2/sqrt(bb*cc*1.0))
    print angle_A,radian2angle(angle_A)

A=beijing
B=nanjing
C=(110.0,22.0)
three_point_angle(A,B,C)
