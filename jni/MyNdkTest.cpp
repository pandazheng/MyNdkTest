#include <string.h>
#include <jni.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <pthread.h>

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
	LOGD("logd---string from MyNdkTest.cpp");
	return (jstring)env->NewStringUTF("string from MyNdkTest.cpp");
}

jint Java_com_example_myndktest_MainActivity_main(JNIEnv* env, jclass thiz){
	return 55;
}

//线程数
#define NUMTHREADS 1
//全局变量

JavaVM *g_jvm = NULL;
jobject g_obj = NULL;

void *thread_fun(void* arg){
	JNIEnv *env;
	jclass cls;
	jmethodID mid;

	if(g_jvm->AttachCurrentThread(&env,NULL) != JNI_OK){
		LOGE("%s: AttachCurrentThread() failed", __FUNCTION__);
		return NULL;
	}

	cls = env->GetObjectClass(g_obj);
	if (cls == NULL) {
		LOGE("FindClass() Error.....");
		goto error;
	}

	mid = env->GetStaticMethodID(cls,"fromJNI","(I)V");
	if (mid == NULL) {
		LOGE("GetMethodID() Error.....");
		goto error;
	}
	env->CallStaticVoidMethod(cls, mid,(int)arg);


error:
	if (g_jvm->DetachCurrentThread() != JNI_OK) {
		LOGE("%s: DetachCurrentThread() failed", __FUNCTION__);
	}
	pthread_exit(0);
}


JNIEXPORT void Java_com_example_myndktest_MainActivity_mainThread(JNIEnv* env, jobject obj){
	int i;
	pthread_t pt[NUMTHREADS];

	for (i = 0; i < NUMTHREADS; i++) {
		pthread_create(&pt[i], NULL, &thread_fun, (void *)i);
	}
}

JNIEXPORT void Java_com_example_myndktest_MainActivity_setJNIEnv(JNIEnv* env, jobject obj){
	env->GetJavaVM(&g_jvm);
	g_obj = env->NewGlobalRef(obj);
}

JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM *vm, void *reserved){
	JNIEnv* env = NULL;
	jint result = -1;

	if (vm->GetEnv((void **)&env, JNI_VERSION_1_4) != JNI_OK) {
		LOGE("GetEnv failed!");
		return result;
	}
	 return JNI_VERSION_1_4;
}







#ifdef __cplusplus
}
#endif
