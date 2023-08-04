package org.example;

import com.jcraft.jsch.UserInfo;
       // import lombok.extern.slf4j.Slf4j;

/**
 * ssh private key passphrase info
 */
// @Slf4j
public class SftpAuthKeyUserInfo implements UserInfo {
    /**
     * ssh private key passphrase
     */
    private String passphrase;

    public SftpAuthKeyUserInfo (String passphrase) {
        this.passphrase = passphrase;
    }

    @Override
    public String getPassphrase() {
        return passphrase;
    }

    @Override
    public String getPassword() {
        return null;
    }

    @Override
    public boolean promptPassphrase(String s) {
        return true;
    }

    @Override
    public boolean promptPassword(String s) {
        return false;
    }

    @Override
    public boolean promptYesNo(String s) {
        return true;
    }

    @Override
    public void showMessage(String message) {
        //log.info ("SSH Message:{}", message);
        System.out.println("SSH Message:"+ message);
    }
}