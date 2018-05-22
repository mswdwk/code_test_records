#!/usr/bin/env python
# coding:utf-8
import os,sys,collections
import os.path

class dir_file_check_repeat:
    file_counter = 0
    file_names = []
    dir_names = []
    def __init__(self, dir):
        self.dir = dir
        self.file_counter += 0
        self.file_names = []
        self.dir_names =[]
        self.dict = {}
        self.dict.clear()
 
    def print_dir_counter(self):
        print "DirName : ", self.dir,  ", fileCounter: ", self.file_counter

    def walk_dir(self):
        rootdir = self.dir
        for parent,dirnames,filenames in os.walk(rootdir):    #三个参数：分别返回1.父目录 2.所有文件夹名字（不含路径） 3.所有文件名字
            #for dirname in  dirnames:                       #输出文件夹信息
            #    print "parent is:" + parent
            #    print  "dirname is" + dirname
            #    print_dir(dirname)
            
            m=collections.Counter(filenames)
            for filename in filenames:                        #输出文件信息
                #print "parent is:" + parent
                #print "filename is:" + filename
                self.file_counter=self.file_counter+1
                
                #self.dict[`filename`] = self.dict[`filename`]+1
                #print "the full name of the file is:" + os.path.join(parent,filename) #输出文件路径信息
        #self.file_names.extend(filenames)
            #print "dirname ",dirnames,", len(filenames) = ",len(filenames)
            print m.values()
        #self.print_dir_counter()
def usage():
    print "Usage: ",sys.argv[0]," path/to/dirname"
    print 
    
if __name__ == "__main__":
    if len(sys.argv) == 1:
        print "[!] Error: illegal arguments"
        usage()
        exit()
    elif len(sys.argv) == 2:
        if os.path.isdir(sys.argv[1]):
            t = dir_file_check_repeat(sys.argv[1])
            t.print_dir_counter()
            t.walk_dir()
            #print t.file_names
            
        else:
            print  sys.argv[1]," is not dir"