import sys
import chardet

def convert_binary_csv_to_utf8(input_file, output_file):
    """
    将二进制编码的CSV文件转换为UTF-8编码的文件
    
    参数:
        input_file: 输入文件路径(二进制编码)
        output_file: 输出文件路径(UTF-8编码)
    """
    # ETX字符(ASCII码3)作为分隔符
    delimiter = b'\x03'
    newline = b'\n'
    csv_field_delimiter=delimiter
    
    try:
        with open(input_file, 'rb') as f_in, open(output_file, 'w', encoding='utf-8') as f_out:
            buffer = b''
            
            while True:
                chunk = f_in.read(4096)
                if not chunk:
                    break
                
                buffer += chunk
                
                while newline in buffer:
                    line_bytes, _, buffer = buffer.partition(newline)
                    if not line_bytes:
                        continue
                    
                    # 分割行
                    fields_bytes = line_bytes.split(delimiter)
                    decoded_fields = []
                    
                    for field_bytes in fields_bytes:
                        # 尝试检测编码
                        try:
                            # 先尝试UTF-8解码
                            decoded = field_bytes.decode('utf-8')
                        except UnicodeDecodeError:
                            try:
                                # 如果UTF-8失败，尝试GBK解码
                                decoded = field_bytes.decode('gbk')
                            except UnicodeDecodeError:
                                # 如果两种编码都失败，使用chardet检测
                                result = chardet.detect(field_bytes)
                                try:
                                    decoded = field_bytes.decode(result['encoding'])
                                except:
                                    # 如果还是失败，使用替代字符
                                    decoded = field_bytes.decode('utf-8', errors='replace')
                        
                        decoded_fields.append(decoded)
                    
                    # 写入UTF-8文件，使用逗号作为分隔符
                    f_out.write(csv_field_delimiter.join(decoded_fields) + '\n')
    
    except FileNotFoundError:
        print(f"错误: 文件 {input_file} 不存在", file=sys.stderr)
        sys.exit(1)
    except Exception as e:
        print(f"处理文件时发生错误: {str(e)}", file=sys.stderr)
        sys.exit(1)

if __name__ == "__main__":
    if len(sys.argv) != 3:
        print("用法: python convert_csv.py <输入文件> <输出文件>", file=sys.stderr)
        sys.exit(1)
    
    input_file = sys.argv[1]
    output_file = sys.argv[2]
    
    convert_binary_csv_to_utf8(input_file, output_file)
    print(f"转换完成，结果已保存到 {output_file}")