package org.example;

import com.jcraft.jsch.ChannelSftp;
import com.jcraft.jsch.JSch;

import com.jcraft.jsch.JSchException;
import com.jcraft.jsch.Session;
// import lombok.extern.slf4j.Slf4j;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import java.io.*;
import java.util.*;

/**
 */
// @Slf4j
public class SftpUtil {
    // public static final Logger log = LoggerFactory.getLogger(SftpUtil.class);
    private String loginName = "user";
    private String loginPassword = "123456";
    private String server = "192.168.79.132";
    private Integer port = 22;


   /* public  void test_client_main(String[] args) {
        SftpUtil sftpUtil = new SftpUtil();
        //上传文件
        sftpUtil.uploadFile();
        //下载文件
        sftpUtil.downloadFile();
        //写文件
        sftpUtil.writeFile();
        //读文件
        sftpUtil.readFile();
        //删除文件
        sftpUtil.deleteFile();
    }*/

    /**
     * 连接登陆远程服务器
     *
     * @return
     */
    public ChannelSftp connect()  {
        JSch jSch = new JSch();
        Session session = null;
        ChannelSftp sftp = null;
        try {
            session = jSch.getSession(loginName, server, port);
            session.setPassword(loginPassword);
            session.setConfig(this.getSshConfig());
            session.setTimeout(3); // 设置timeout时间
            session.connect();

            sftp = (ChannelSftp)session.openChannel("sftp");
            sftp.connect();
            //log.error("结果："+session.equals(sftp.getSession()));
            //log.info("登录成功:" + sftp.getServerVersion());
            System.out.println("login success: ["+server+":"+port+"]");

        } catch (JSchException e){
            System.err.println("JSch connect ["+server+":"+port+"] failed: "+e.toString());
            e.printStackTrace();
            return null;
        }
        catch (Exception e) {
            // log.error("SSH方式连接FTP服务器时有JSchException异常!",e);
            System.err.println("connect ["+server+":"+port+"] failed: "+e.toString());
            e.printStackTrace();
            return null;
        }
        return sftp;
    }

    public ChannelSftp connect_by_pubkey(String keyLocation)  {
        JSch jsch = new JSch();
        Session session = null;
        ChannelSftp sftp = null;
        try {
            jsch.addIdentity("C:\\Users\\lenovo\\.ssh\\id_rsa");
            jsch.setKnownHosts("C:\\Users\\lenovo\\.ssh\\known_hosts");

            // set up session
            session = jsch.getSession(loginName,server,port);

            // use private key instead of username/password
            session.setConfig(
                    "PreferredAuthentications",
                    "publickey,keyboard-interactive,password");
            session.setConfig("kex", "diffie-hellman-group1-sha1");
            session.setConfig("StrictHostKeyChecking", "no");
            session.setTimeout(5);
            System.out.println("user info:"+session.getUserName());
            //跳过Kerberos username 身份验证提示
            // session.setConfig("PreferredAuthentications", "publickey,keyboard-interactive,password");

           /* java.util.Properties config = new java.util.Properties();
            config.put("StrictHostKeyChecking", "no");
            session.setConfig(config);
            session.setUserInfo(new SftpAuthKeyUserInfo(""));*/
            session.connect();

            sftp = (ChannelSftp)session.openChannel("sftp");
            sftp.connect();
            //log.error("结果："+session.equals(sftp.getSession()));
            //log.info("登录成功:" + sftp.getServerVersion());
            System.out.println("login success: ["+server+":"+port+"]");

        } catch (JSchException e){
            System.err.println("JSch connect ["+server+":"+port+"] failed: "+e.toString());
            e.printStackTrace();
            return null;
        }
        catch (Exception e) {
            // log.error("SSH方式连接FTP服务器时有JSchException异常!",e);
            System.err.println("connect ["+server+":"+port+"] failed: "+e.toString());
            e.printStackTrace();
            return null;
        }
        return sftp;
    }

    /**
     * 获取服务配置
     * @return
     */
    private Properties getSshConfig() {
        Properties sshConfig =  new Properties();
        sshConfig.put("StrictHostKeyChecking", "no"); // jdk 1.8, "no" is ok
        return sshConfig;
    }

    /**
     * 关闭连接
     * @param sftp
     */
    public void disconnect(ChannelSftp sftp) {
        try {
            if(sftp!=null){
                if(sftp.getSession().isConnected()){
                    sftp.getSession().disconnect();
                }
            }
        } catch (Exception e) {
           // log.error("关闭与sftp服务器会话连接异常",e);
            e.printStackTrace();
        }
    }

    /**
     * 下载远程sftp服务器文件
     *
     * @return
     */
    public void downloadFile(String localFilename,String remoteFilename) {
        FileOutputStream output = null;
        ChannelSftp sftp = null;
        try {
            // sftp = connect();
            String localKeyFile = "C:\\Users\\lenovo\\Desktop\\id_rsa";
            // String localKey ="C:\\Users\\lenovo\\Desktop\\ssh_host_dsa_key.pub";
            sftp = connect_by_pubkey(localKeyFile);
            if(sftp == null){
                return ;
            }
            //sftp服务器上文件路径
            // String remoteFilename = "/test1/测试.txt";
            //下载至本地路径
            File localFile = new File(localFilename);
            output = new FileOutputStream(localFile);

            sftp.get(remoteFilename, output);
            System.out.println("成功接收文件,本地路径：" + localFile.getAbsolutePath());
        } catch (Exception e) {
            // log.error("接收文件异常!",e);
            e.printStackTrace();
            System.err.println("download file failed:"+remoteFilename);
        } finally {
            try {
                if (null != output) {
                    output.flush();
                    output.close();
                }
                // 关闭连接
                disconnect(sftp);
            } catch (IOException e) {
               // log.error("关闭文件时出错!",e);
                e.printStackTrace();
            }
        }
    }



    /**
     * 读取远程sftp服务器文件
     *
     * @return
     */
    /*public void readFile() {
        InputStream in = null;
        ArrayList<String> strings = new ArrayList<>();
        ChannelSftp sftp = null;
        try {
            sftp = connect();
            if(sftp == null){
                return;
            }
            String remotePath = "/test1/";
            String remoteFilename = "测试1.txt";
            sftp.cd(remotePath);
            if(!listFiles(remotePath).contains(remoteFilename)){
                log.error("no such file");
                return;
            }
            in = sftp.get(remoteFilename);
            if (in != null) {
                BufferedReader br = new BufferedReader(new InputStreamReader(in,"utf-8"));
                String str = null;
                while ((str = br.readLine()) != null) {
                    System.out.println(str);
                }
            }else{
                log.error("in为空，不能读取。");
            }
        } catch (Exception e) {
            log.error("接收文件时异常!",e);
        }  finally {
            try {
                if(in !=null){
                    in.close();
                }
                // 关闭连接
                disconnect(sftp);
            } catch (Exception e) {
                log.error("关闭文件流时出现异常!",e);
            }
        }
    }*/


    /**
     * 写文件至远程sftp服务器
     *
     * @return
     */
    public void writeFile(){
        ChannelSftp sftp = null;
        ByteArrayInputStream input = null;
        try {
            sftp = connect();
            if(sftp == null){
                return;
            }
            // 更改服务器目录
            String remotePath = "/test1/";
            sftp.cd(remotePath);
            // 发送文件
            String remoteFilename = "写文件.txt";
            String content = "测试内容";
            input = new ByteArrayInputStream(content.getBytes());
            sftp.put(input, remoteFilename);
        } catch (Exception e) {
            // log.error("发送文件时有异常!",e);
            e.printStackTrace();
        } finally {
            try {
                if (null != input) {
                    input.close();
                }
                // 关闭连接
                disconnect(sftp);
            } catch (Exception e) {
                // log.error("关闭文件时出错!",e);
                e.printStackTrace();
            }
        }
    }

    /**
     * 上传文件至sftp服务器
     * @return
     */
    public void uploadFile(String localFileName,String remoteFilename) {
        FileInputStream fis = null;
        ChannelSftp sftp = null;
        // 上传文件至服务器此目录
        // String remotePath = "./file/sftp/从sftp服务器上下载.txt";
        // String remoteFilename = "/test1/上传至sftp服务器.txt";
        try {
            sftp = connect();
            if(sftp == null){
                return ;
            }

            File localFile = new File(localFileName);
            fis = new FileInputStream(localFile);
            //发送文件
            sftp.put(fis, remoteFilename);
            // log.info("成功上传文件" + localFileName);
            System.out.println("upload success: "+localFileName);
        } catch (Exception e) {
            // log.error("上传文件时异常!",e);
            e.printStackTrace();
            System.err.println("upload failed : "+localFileName);
        } finally {
            try {
                if (fis != null) {
                    fis.close();
                }
                // 关闭连接
                disconnect(sftp);
            } catch (Exception e) {
                // log.error("关闭文件时出错!",e);
                e.printStackTrace();
            }
        }
    }


    /**
     * 遍历远程文件
     *
     * @param remotePath
     * @return
     * @throws Exception
     */
    public List<String> listFiles(String remotePath){
        List<String> ftpFileNameList = new ArrayList<String>();
        ChannelSftp.LsEntry isEntity = null;
        String fileName = null;
        ChannelSftp sftp = null;
        try{
            sftp = connect();
            if(sftp == null){
                return null;
            }
            Vector<ChannelSftp.LsEntry> sftpFile = sftp.ls(remotePath);
            Iterator<ChannelSftp.LsEntry> sftpFileNames = sftpFile.iterator();
            while (sftpFileNames.hasNext()) {
                isEntity = (ChannelSftp.LsEntry) sftpFileNames.next();
                fileName = isEntity.getFilename();
                ftpFileNameList.add(fileName);
            }
            return ftpFileNameList;
        }catch (Exception e){
            // log.error("遍历查询sftp服务器上文件异常",e);
            e.printStackTrace();
            return null;
        }finally {
            disconnect(sftp);
        }

    }


    /**
     * 删除远程文件
     * @return
     */
    public void deleteFile() {
        boolean success = false;
        ChannelSftp sftp = null;
        try {
            sftp = connect();
            if(sftp == null){
                return;
            }
            String remotePath = "/test1/";
            String remoteFilename = "limit.lua";
            // 更改服务器目录
            sftp.cd(remotePath);
            //判断文件是否存在
            if(listFiles(remotePath).contains(remoteFilename)){
                // 删除文件
                sftp.rm(remoteFilename);
                // log.info("删除远程文件" + remoteFilename + "成功!");
                System.out.println("delete remote file ok:"+remoteFilename);
            }

        } catch (Exception e) {
            // log.error("删除文件时有异常!",e);
            e.printStackTrace();
        } finally {
            // 关闭连接
            disconnect(sftp);
        }
    }
}
