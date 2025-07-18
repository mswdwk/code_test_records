use std::{
    fs::File,
    io::{BufRead, BufReader, BufWriter, Write},
    path::Path,
};

use encoding_rs::{GB18030, GBK, UTF_8};
use encoding_rs_io::DecodeReaderBytesBuilder;

fn main() {
    let args: Vec<String> = std::env::args().collect();
    if args.len() != 3 {
        eprintln!("Usage: {} <input_file> <output_file>", args[0]);
        std::process::exit(1);
    }

    let input_path = Path::new(&args[1]);
    let output_path = Path::new(&args[2]);

    if let Err(e) = convert_csv(input_path, output_path) {
        eprintln!("Error: {}", e);
        std::process::exit(1);
    }

    println!(
        "Conversion completed successfully. Output saved to {}",
        output_path.display()
    );
}

fn convert_csv(input_path: &Path, output_path: &Path) -> Result<(), Box<dyn std::error::Error>> {
    // 打开输入文件
    let input_file = File::open(input_path)?;
    
    // 创建解码器，首先尝试UTF-8
    let reader = BufReader::new(
        DecodeReaderBytesBuilder::new()
            .encoding(Some(UTF_8))
            .build(input_file),
    );

    // 创建输出文件，使用UTF-8编码
    let output_file = File::create(output_path)?;
    let mut writer = BufWriter::new(output_file);

    // ETX分隔符(ASCII码3)
    const DELIMITER: char = 0x03 as char;
    const NEWLINE: char = '\n';

    for line in reader.lines() {
        let line = line?;
        
        // 分割字段
        let fields: Vec<&str> = line.split(DELIMITER).collect();
        
        // 处理每个字段的编码
        let mut processed_fields = Vec::with_capacity(fields.len());
        
        for field in fields {
            // 检查是否包含替换字符（表示解码有问题）
            if field.contains('\u{FFFD}') {
                // 尝试从原始字节重新解码
                if let Some(decoded) = try_decode_alternative(field.as_bytes()) {
                    processed_fields.push(decoded);
                    continue;
                }
            }
            
            // 如果没有解码问题，直接使用
            processed_fields.push(field.to_string());
        }
        
        // 写入输出文件，用ETX分隔
        let output_line = processed_fields.join(&DELIMITER.to_string());
        writeln!(writer, "{}", output_line)?;
    }

    writer.flush()?;
    Ok(())
}

fn try_decode_alternative(bytes: &[u8]) -> Option<String> {
    // 尝试UTF-8解码
    if let Ok(s) = String::from_utf8(bytes.to_vec()) {
        if !s.contains('\u{FFFD}') {
            return Some(s);
        }
    }

    // 尝试GB18030解码
    let (decoded, _, had_errors) = GB18030.decode(bytes);
    if !had_errors {
        return Some(decoded.into_owned());
    }

    // 尝试GBK解码
    let (decoded, _, had_errors) = GBK.decode(bytes);
    if !had_errors {
        return Some(decoded.into_owned());
    }

    // 所有解码尝试都失败，返回None
    None
}