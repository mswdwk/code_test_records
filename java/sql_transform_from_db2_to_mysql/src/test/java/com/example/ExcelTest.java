package com.example;

import org.apache.poi.ss.usermodel.Cell;
import org.apache.poi.ss.usermodel.Row;
import org.apache.poi.ss.usermodel.Sheet;
import org.apache.poi.ss.usermodel.Workbook;
import org.apache.poi.xssf.usermodel.XSSFWorkbook;
import org.junit.Test;

import java.io.FileOutputStream;
import java.io.IOException;

public class ExcelTest {

    @Test
    public void t1(){
        // 创建一个Excel工作簿
        Workbook workbook = new XSSFWorkbook();
        // 创建一个工作表sheet
        Sheet sheet = workbook.createSheet("Example Sheet");

        // 创建行（Row）和单元格（Cell）
        Row row = sheet.createRow(0); // 创建第一行
        Cell cell = row.createCell(0); // 在第一行创建第一列的单元格
        cell.setCellValue("Hello, World!"); // 设置单元格的值

        cell = row.createCell(1); // 在第一行创建第一列的单元格
        cell.setCellValue("这是第2列"); // 设置单元格的值

        // 创建更多的行和单元格并填充数据
        for (int i = 1; i < 66000; i++) {
            Row currentRow = sheet.createRow(i);
            Cell col1 = currentRow.createCell(0); // 第一列
            Cell col2 = currentRow.createCell(1); // 第二列
            col1.setCellValue("Row " + i); // 第一列的值
            col2.setCellValue(i * 2); // 第二列的值（行号的两倍）
        }

        // 将Excel写入文件系统
        try (FileOutputStream outputStream = new FileOutputStream("example.xlsx")) {
            workbook.write(outputStream);
            workbook.close(); // 关闭工作簿释放资源
            System.out.println("Excel文件已成功创建！");
        } catch (IOException e) {
            e.printStackTrace();
        }
    }
}
