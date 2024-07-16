# *_*coding:utf-8 *_*
# @Time    : 2024/2/23

import json,base64

if __name__ == '__main__':
    # 要编码的数据
    data = {"uname":"Tanch","uid":3}
    # 先转化为bytes类型数据
    data_bytes = json.dumps(data).encode()
    print(type(data_bytes))
    # 编码
    base_data = base64.b64encode(data_bytes)
    print(base_data,",len=",len(base_data))

    # 解码
    string_bytes = b"eyJ1bmFtZSI6ICJUYW5jaCIsICJ1aWQiOiAzfQ=="
    ori_data = base64.b64decode(string_bytes).decode()
    print(string_bytes,",len=",len(string_bytes))
    # 字符串
    print(ori_data,",len=",len(ori_data))
    # 变回原来的字典
    data = json.loads(ori_data)
    print(type(data))