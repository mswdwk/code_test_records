#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <jni.h>

JavaVM *jvm;
JNIEnv *env;

void JVM_Init()
{
    JavaVMInitArgs vm_args;
    JavaVMOption options[1];

    vm_args.version = JNI_VERSION_1_6;
    //vm_args.ignoreUnrecognized = false;
    vm_args.ignoreUnrecognized = JNI_FALSE;
    vm_args.nOptions = 0;

    char classpath[1024] ={0}; 
    // char *env_classpath = getenv("CLASSPATH");
     char *env_java_home = getenv("JAVA_HOME");

     if (env_java_home) {
	sprintf(classpath,"-Djava.class.path=%s/lib:.",env_java_home);
         options[0].optionString = classpath;
        vm_args.nOptions++;
     }

    if (vm_args.nOptions > 0) {
        vm_args.options = options;
    }

    // create jvm
    jint res = JNI_CreateJavaVM(&jvm, (void **)&env, &vm_args);
    // delete options;
    if (res < 0) {
        printf("Create Java VM error, code = %d\n", res);
        exit(-1);
    }
    printf("Create Java VM successful!, code = %d\n", res);
}

void JVM_Destroy()
{
    //jvm->DestroyJavaVM();
   (*jvm)->DestroyJavaVM(jvm);

    env = NULL;
    jvm = NULL;
}

int main(){
    printf("Hello World!");
    JVM_Init();
    printf("init jvm !");
//	JVM_Destroy();
}
