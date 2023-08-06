package org.example;


/**
 * Hello world!
 *
 */
public class App 
{
    public static void main( String[] args )
    {
        System.out.println( "Hello World!" );
        if ( args.length < 4){
            System.out.println("Usage: host port loginname");
            return;
        }
        String host = args[0];
        int port = Integer.parseInt(args[1]);
        String loginName = args[2];
        String prikeyPath = args[3];
        SftpUtil sftpUtil = new SftpUtil(host,port,loginName,prikeyPath);
        //上传文件
        // sftpUtil.uploadFile("C:\\Users\\lenovo\\Downloads\\instr.sql","1.txt");
        //下载文件
        sftpUtil.downloadFile("1_0_2.txt","1.txt");
        //写文件
        // sftpUtil.writeFile();
        //读文件
        // sftpUtil.readFile();
        //删除文件
        // sftpUtil.deleteFile();
    }
}
