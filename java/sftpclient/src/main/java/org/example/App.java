package org.example;

// import static org.example.SftpUtil.test_client_main;

/**
 * Hello world!
 *
 */
public class App 
{
    public static void main( String[] args )
    {
        System.out.println( "Hello World!" );
        SftpUtil sftpUtil = new SftpUtil();
        //上传文件
        sftpUtil.uploadFile("C:\\Users\\lenovo\\Downloads\\instr.sql","1.txt");
        //下载文件
        sftpUtil.downloadFile("1_0.txt","1.txt");
        //写文件
        // sftpUtil.writeFile();
        //读文件
        // sftpUtil.readFile();
        //删除文件
        // sftpUtil.deleteFile();
    }
}
