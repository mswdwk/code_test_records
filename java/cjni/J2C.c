#include <stdio.h>

#include "J2C.h"

JNIEXPORT int JNICALL Java_J2C_write2proc(JNIEnv * env, jobject arg, jint pid)
{

     printf("current pid is %d\n", pid);

     return 0;

}
