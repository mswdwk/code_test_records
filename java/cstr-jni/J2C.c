#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "J2C.h"

// java中的jstring, 转化为c的一个字符数组  
char*   Jstring2CStr(JNIEnv*   env,   jstring   jstr)  
{  
     char*   rtn   =   NULL;  
     jclass   clsstring   =   (*env)->FindClass(env,"java/lang/String");  
     jstring   strencode   =   (*env)->NewStringUTF(env,"UTF8");  
     jmethodID   mid   =   (*env)->GetMethodID(env,clsstring,   "getBytes",   "(Ljava/lang/String;)[B");  
     jbyteArray   barr=   (jbyteArray)(*env)->CallObjectMethod(env,jstr,mid,strencode); // String .getByte("GB2312");  
     jsize   alen   =   (*env)->GetArrayLength(env,barr);  
     jbyte*   ba   =   (*env)->GetByteArrayElements(env,barr,JNI_FALSE);  
     if(alen   >   0)  
     {  
      rtn   =   (char*)malloc(alen+1);         //new   char[alen+1]; "\0"  
      memcpy(rtn,ba,alen);  
      rtn[alen]=0;  
     }  
     (*env)->ReleaseByteArrayElements(env,barr,ba,0);  //释放内存  
  
     return rtn;  
} 

JNIEXPORT jint JNICALL Java_J2C_write2proc(JNIEnv * env, jobject arg, jlong pid)
{

     printf("current pid is %d\n", pid);

     return 0;

}

JNIEXPORT jstring JNICALL Java_J2C_testJstring(JNIEnv *env, jobject arg, jstring str){
 	char* p =  Jstring2CStr(env,str);  
    	char* newstr = "append string"; 
    	return (*env)->NewStringUTF(env, strcat(p,newstr));  
}

