#coding:utf-8
s = "你好，中国！" 
print(s)     # Python2输出乱码，Python3正常输出
print(type(s))    # 均输出 <type 'str'>

#解码成unicode
s1 = s.decode("utf-8")
print(s1)    # Python2中输出 “你好，中国！”，Python3显示'str'对象没有属性'decode'
print(type(s1))   # Python2中输出 <type 'unicode'>  Python3中输出 <class 'str'> 

#编码成gbk 或 utf-8
s2 = s1.encode('gbk')
print(s2)      # Python2中输出 “你好，中国！”
print(type(s2))    # Python2中输出 <type 'str'>
s3 = s1.encode('utf-8')
print(s3)      # Python2输出乱码，
print(type(s3))    # 输出 <type 'str'>
    
