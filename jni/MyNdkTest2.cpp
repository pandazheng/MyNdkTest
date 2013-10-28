#include <string.h>
#include <jni.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#ifdef __cplusplus
extern "C" {
#endif

#include <android/log.h>
#define LOG_TAG    "myndktest -- JNILOG" // 这个是自定义的LOG的标识
//#undef LOG // 取消默认的LOG
#define LOGI(...)  __android_log_print(ANDROID_LOG_INFO,LOG_TAG, __VA_ARGS__)
#define LOGD(...)  __android_log_print(ANDROID_LOG_DEBUG,LOG_TAG, __VA_ARGS__)
#define LOGW(...)  __android_log_print(ANDROID_LOG_WARN,LOG_TAG, __VA_ARGS__)
#define LOGE(...)  __android_log_print(ANDROID_LOG_ERROR,LOG_TAG, __VA_ARGS__)
#define LOGF(...)  __android_log_print(ANDROID_LOG_FATAL,LOG_TAG, __VA_ARGS__)

jstring Java_com_example_myndktest_MainActivity_stringFromJNI(JNIEnv* env, jclass thiz){
//	for(int i=0;i<100;i++){
//		LOGD("logd---string from MyNdkTest.cpp");
//		sleep(1000);
//	}
	LOGD("logd---string from MyNdkTest.cpp");
	return (jstring)env->NewStringUTF("string from MyNdkTest.cpp");
}

jint Java_com_example_myndktest_MainActivity_main(JNIEnv* env, jclass thiz){
	return 55;
}

int main(int argc, char* arg[]){
	LOGD("========from my int main()");
	printf("this is main");
	return 0;
}

#ifdef __cplusplus
}
#endif
