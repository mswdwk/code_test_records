extern "C"
JNIEXPORT jstring JNICALL
Java_J2C_testJstring(JNIEnv *env, jobject instance,jstring str_) {
    char* p =  Jstring2CStr(env,str_);  
    char* newstr = "append string";  
    //strcat(dest, sorce) 把sorce字符串添加到dest字符串的后面  
    return (*env)->NewStringUTF(env, strcat(p,newstr));
}
