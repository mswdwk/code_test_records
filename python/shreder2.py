#!/usr/bin/env python
# coding:utf-8
#
# Build By LandGrey 2016-10-20
#
# function: shredding single file or directory
#

import glob, os, sys, stat
import random,shutil,string,traceback

const_str=""
line_len = 4096*32
def shreder_dir(directory, rewritecounts=1):
    global filepaths
    global dirpaths
    print
    print ("[+] Shredding '%s' ..." % (directory))
    try:
        newdirectoryname = os.path.join(os.path.dirname(directory), "".join(chr(random.randint(97, 122)) for _ in range(1, 6)))
        print ("newdirectoryname:",newdirectoryname)
        os.rename(directory, newdirectoryname)
        directory = newdirectoryname
    except:
        print ("[!] Error: cannot rename root directory name")
        traceback.print_exc()
        exit()

    for rootpath, subdirsname, filenames in os.walk(directory):
        # get all directories
        dirpaths.extend([os.path.abspath(os.path.join(rootpath, _)) for _ in subdirsname])
        # get all absolute file path
        filepaths.extend([os.path.abspath(os.path.join(rootpath, _)) for _ in filenames])

    print ("[+] Chmod the file")
    for filepath in filepaths:
        try:
            os.chmod(filepath, stat.S_IREAD | stat.S_IWRITE)
        except:
            pass

    print ("[+] Writing random data ...")
    for _ in range(rewritecounts):
        print ("[+] Rewrite count: %d" % (_+1))
        for filepath in filepaths:
            rewrite(filepath)

    print ("[+] Truncating files: reset file content length -> 0")
    for filepath in filepaths:
        truncating(filepath)

    print ("[+] Random renaming file names")
    for filepath in filepaths:
        renamefile(filepath)
    print ("[+] Random renaming directory names")
    renamedir(dirpaths)

    print ("[+] Deleting the whole directory tree")
    #
    os.chdir(os.path.join(directory, ".."))
    try:
        shutil.rmtree(directory)
    except (OSError, ex):
        print ("[!] Error: Cannot removing directory: '%s' " % directory)
        traceback.print_exc()
    print
    print ("[+] Completely!")


def shreder_file(filepath, rewritecounts=1):
    print
    print ("[+] Shredding '%s' ..." % filepath)
    print ("[+] Chmod the file")
    try:
        os.chmod(filepath, stat.S_IREAD | stat.S_IWRITE)
    except:
        pass

    print ("[+] Writing random data ...")
    for _ in range(rewritecounts):
        print ("[+] Rewrite count: %d" % (_+1))
        rewrite(filepath)

    print ("[+] Truncating files: reset file content length -> 0")
    truncating(filepath)

    print ("[+] Random renaming file names")
    newname = renamefile(filepath)

    print ("[+] Deleting the file")
    os.remove(newname)

    print
    print ("[+] Completely!")

def create_const_str(length,num):
    _str=""
    #print chr(num)
    for _ in range(length):
        #const_str.join(chr(num))
        _str+=chr(random.randint(32,126))
        #_str+=chr(num)
    return _str

def write0(file,size):
    loop = size/line_len
    remainder = size % line_len
    for _ in range(loop):
        percent=_*100/loop
        print ("process ",percent,"%,loop times ",_,"\r",)
        file.write(const_str)
    file.write("".join(chr(random.randint(32,126))) for _ in range(remainder))

def rewrite(filepath):
    try:
        filesize = os.path.getsize(filepath)
        print ("rewrite file ",filepath," size ",filesize)
        with open(filepath, "w+b") as f:
            #f.write("".join(chr(random.randint(0, 255)) for _ in xrange(filesize)))
            #f.write("".join(chr(0) for _ in xrange(filesize)))
            write0(f,filesize)
    except:
        pass


def truncating(filepath):
    # default: 2 times
    for _ in range(2):
        try:
            with open(filepath, 'w') as f:
                pass
        except:
            pass

 
def renamefile(filepath):
    #newname = os.path.join(os.path.dirname(filepath), "".join(random.sample(string.ascii_letters, random.randint(4, 8))))
    newname = os.path.join(os.path.dirname(filepath),"".join(random.sample(string.ascii_letters, 1)))
    try:
        os.rename(filepath, newname)
    except:
        pass
    return newname


def renamedir(dirpaths):
    # equals: dirpaths.sort()
    # dirpaths.sort(cmp=lambda x, y: y.count(os.path.sep) - x.count(os.path.sep))
    dirpaths.sort()
    for dirpath in dirpaths:
        try:
            os.rename(dirpath, os.path.join(os.path.dirname(dirpath), "".join(random.sample(string.ascii_letters, random.randint(4, 8)))))
        except:
            pass


def usage():
    print
    print ("[*] Usage: python shreder.py [dirname or filename] [rewrite-counts]")
    print ("[*] Rewrite-counts: default -> 1, allow not to fill in")
    print (r"[*] Example: python shreder.py D:\tests")
    print (r"             python shreder.py D:\tests\test.doc 5")
    print (r"             python shreder.py suffix") # file suffix
    print


def somefiles(suffix,files):
    dir = os.path.dirname(os.path.realpath(__file__)) 
    f = glob.glob(dir + '\\*.' + suffix) 
    for file in f:
        filename = os.path.basename(file)
        files.append(file)
    print ("-----")
    print ("",files,"")
    print ("-----")
    if len(files):
        return 1
    else: 
        return 0

if __name__ == "__main__":
    filepaths = []
    dirpaths = []
    files = []
    for i in range(len(sys.argv)):
        print (i," : ",sys.argv[i])
    const_str = create_const_str(line_len,50)
    print ("usage:",usage())
    if len(sys.argv) == 1:
        print ("[!] Error: illegal arguments")
        usage()
        exit()
    elif len(sys.argv) == 2:
        if os.path.isdir(sys.argv[1]):
            shreder_dir(sys.argv[1])
        elif os.path.isfile(sys.argv[1]):
            shreder_file(sys.argv[1])
        elif somefiles(sys.argv[1],files):
            for file in files:
                shreder_file(file)
        else:
            print ("[!] Error: '%s' is not exist" % sys.argv[1])
            usage()
            exit()
    elif len(sys.argv) == 3:
        if os.path.isdir(sys.argv[1]):
            if str(sys.argv[2]).isdigit():
                shreder_dir(sys.argv[1], int(sys.argv[2]))
            else:
                print ("[?] Warn: '%s' is not a legal digit, default use: 1" % sys.argv[2])
                shreder_dir(sys.argv[1])
        elif os.path.isfile(sys.argv[1]):
            if str(sys.argv[2]).isdigit():
                shreder_file(sys.argv[1], int(sys.argv[2]))
            else:
                print ("[?] Warn: '%s' is not a legal digit, default use: 1" % sys.argv[2])
                shreder_file(sys.argv[1])
        else:
            print ("[!] Error: %s is not a legal directory or file" % sys.argv[1])
            usage()
            exit()