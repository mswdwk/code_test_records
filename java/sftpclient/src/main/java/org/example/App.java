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
        if ( args.length < 7){
            System.out.println("Usage: ./App.jar loginType host port loginname password/privateKeyPath " +
                    "upload/download/delete Filename");
            System.out.println("         loginType: 1 -> user passsword \n\t 2-> private Key");
            return;
        }
        String loginType = args[0];
        String host = args[1];
        int port = Integer.parseInt(args[2]);
        String loginName = args[3];
        String prikeyPath = args[4];
        String operate = args[5];
        String fileName = args[6];
        SftpUtil sftpUtil = new SftpUtil(loginType,host,port,loginName,prikeyPath);

        if ( operate.equals( "upload")) {
            //上传文件
            sftpUtil.uploadFile(fileName,"1.txt");
        } else if (operate.equals( "download")){
            //下载文件
            sftpUtil.downloadFile("1_0.txt",fileName);
        } else if ( operate.equals( "delete" )) {
            //删除文件
            sftpUtil.deleteFile(".",fileName);
        } else {
            System.err.println("Unknown operator : " + operate);
        }

        //写文件
        // sftpUtil.writeFile();
        //读文件
        // sftpUtil.readFile();
    }
}
