# https://www.cnblogs.com/ttj57/p/13590691.html
import json#导入json库
import openpyxl
from openpyxl import Workbook

#excel转json
def excel_to_json(excel_file,json_file):
    wb=openpyxl.load_workbook(excel_file)#读取excel文件
    excel_data={}#定义字典excel_data存储每个表的数据{表名:数据}
    for sheet in wb.sheetnames:
        result = []  # 定义列表result存储所有读取数据
        for rows in wb[sheet]:#获取表的每一行数据
            tmp=[]#定义列表tmp存储每一行的数据
            for cell in rows:#遍历一行每个单元格的数据
                tmp.append(cell.value)
            result.append(tmp)
        excel_data[sheet]=result
    #覆盖写入json文件
    with open(json_file, mode='w', encoding='utf-8') as jf:
        json.dump(excel_data, jf, indent=2, sort_keys=True, ensure_ascii=False)

excel_to_json(r'./test.xlsx',r'./test.json')#调用函数，传入参数