package org.example;
import java.io.File;
import java.util.List;
import java.util.ArrayList;

public class CreateFileThread implements Runnable {
    private Thread t;
    private String threadName;
	private int loop_count;

    CreateFileThread( String name,int count) {
        threadName = name;
		loop_count = count;
        System.out.println("Creating " +  threadName+" loop_count "+loop_count );
    }

    public void run() {
        System.out.println("Running " +  threadName );
        String filename="";
		List<File> files = new ArrayList<File>();
        try {
            for(int i = 0;i<loop_count; i++) {
                filename = "filename_"+String.valueOf(i);	
                File file =  new File(filename);
				files.add(file);
            	if(file.createNewFile()){
                	System.out.printf("文件创建成功！%s\r",filename);
            	}else{
                	System.out.println("create file failed:"+filename);
					break;
				}
                // 让线程睡眠一会
                Thread.sleep(10);
            }
        }catch (Exception e) {
            System.out.println("Thread " +  threadName + " interrupted."+filename);
			e.printStackTrace();
        }
        System.out.println("\nThread " +  threadName + " exiting.");
    }

    public void start () {
        System.out.println("Starting " +  threadName );
        if (t == null) {
            t = new Thread (this, threadName);
            t.start ();
        }
    }
}
