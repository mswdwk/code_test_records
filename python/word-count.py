#coding=utf-8

import collections

str1=['a','b','c','d','a','a','b','c']
m=collections.Counter(str1)

print str1
print m
print m['a']#字符a出现的次数
print m['b']#字符b出现的次数