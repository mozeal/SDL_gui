#include <jni.h>
#include <android/log.h>
#include <android/asset_manager.h>
#include <android/asset_manager_jni.h>
#include "SDL_config.h"
#include "SDL_stdinc.h"

AAssetManager* pAssetManager = NULL;
JNIEnv* pEnv = NULL;
jclass jni_cls;
jobject jni_obj;
 
//static float fLastAccelerometer[3];
extern "C" {
    void Java_org_libsdl_app_SDLActivity_nativeSetupJNI(JNIEnv* mEnv, jclass cls);
    void Java_org_libsdl_app_SDLAudioManager_nativeSetupJNI(JNIEnv* mEnv, jclass cls);
    void Java_org_libsdl_app_SDLControllerManager_nativeSetupJNI(JNIEnv* mEnv, jclass cls);

    void Java_org_libsdl_app_SDLActivity_onNativeDropFile(JNIEnv* env, jclass jcls, jstring filename);
    void Java_org_libsdl_app_SDLActivity_onNativeResize(JNIEnv* env, jclass jcls, jint width, jint height, jint format, jfloat rate);
    jint Java_org_libsdl_app_SDLControllerManager_onNativePadDown(JNIEnv* env, jclass jcls, jint device_id, jint keycode);
    jint Java_org_libsdl_app_SDLControllerManager_onNativePadUp(JNIEnv* env, jclass jcls, jint device_id, jint keycode);
    void Java_org_libsdl_app_SDLControllerManager_onNativeJoy(JNIEnv* env, jclass jcls, jint device_id, jint axis, jfloat value);
    void Java_org_libsdl_app_SDLControllerManager_onNativeHat(JNIEnv* env, jclass jcls, jint device_id, jint hat_id, jint x, jint y);
    jint Java_org_libsdl_app_SDLControllerManager_nativeAddJoystick(JNIEnv* env, jclass jcls, jint device_id, jstring device_name, jint is_accelerometer, jint nbuttons, jint naxes, jint nhats, jint nballs);
    jint Java_org_libsdl_app_SDLControllerManager_nativeRemoveJoystick(JNIEnv* env, jclass jcls, jint device_id);
    void Java_org_libsdl_app_SDLActivity_onNativeSurfaceChanged(JNIEnv* env, jclass jcls);
    void Java_org_libsdl_app_SDLActivity_onNativeSurfaceDestroyed(JNIEnv* env, jclass jcls);

    void Java_org_libsdl_app_SDLActivity_onNativeKeyDown(JNIEnv* env, jclass jcls, jint keycode);
    void Java_org_libsdl_app_SDLActivity_onNativeKeyUp(JNIEnv* env, jclass jcls, jint keycode);
    void Java_org_libsdl_app_SDLActivity_onNativeKeyboardFocusLost(JNIEnv* env, jclass jcls);
    void Java_org_libsdl_app_SDLActivity_onNativeTouch(JNIEnv* env, jclass jcls, jint touch_device_id_in, jint pointer_finger_id_in, jint action, jfloat x, jfloat y, jfloat p);
    void Java_org_libsdl_app_SDLActivity_onNativeMouse(JNIEnv* env, jclass jcls, jint button, jint action, jfloat x, jfloat y);
    void Java_org_libsdl_app_SDLActivity_onNativeAccel(JNIEnv* env, jclass jcls, jfloat x, jfloat y, jfloat z);

    void Java_org_libsdl_app_SDLActivity_nativeLowMemory(JNIEnv* env, jclass cls);
    void Java_org_libsdl_app_SDLActivity_nativeQuit(JNIEnv* env, jclass cls);
    void Java_org_libsdl_app_SDLActivity_nativePause(JNIEnv* env, jclass cls);
    void Java_org_libsdl_app_SDLActivity_nativeResume(JNIEnv* env, jclass cls);

    void Java_org_libsdl_app_SDLInputConnection_nativeCommitText(JNIEnv* env, jclass cls, jstring text, jint newCursorPosition);
    void Java_org_libsdl_app_SDLInputConnection_nativeSetComposingText(JNIEnv* env, jclass cls, jstring text, jint newCursorPosition);
    jstring Java_org_libsdl_app_SDLActivity_nativeGetHint(JNIEnv* env, jclass cls, jstring name);
};

char __directoryList[512][128];
int __numFilesInDirectory;

extern "C" void Java_com_jimmysoftware_clearscreen_SDLActivity_addDirectoryList(JNIEnv* env, jobject obj, jstring filename){
    __android_log_print(ANDROID_LOG_VERBOSE, "__GUI__", "****** in addDirectoryList *****" );
    const char *str = env->GetStringUTFChars( filename, 0);
    __android_log_print(ANDROID_LOG_VERBOSE, "__GUI__", "****** %s", str );
    strcpy( __directoryList[__numFilesInDirectory++], str );
    env->ReleaseStringUTFChars(filename, str);
};

extern "C" void Java_com_jimmysoftware_clearscreen_SDLActivity_nativeDemoInit(JNIEnv* env, jobject obj){
    jni_obj = obj;
    pEnv = env;
    
    __android_log_print(ANDROID_LOG_VERBOSE, "__GUI__", "****** in nativeDemoInit *****" );
   jclass cls = env->GetObjectClass(obj);  // instead of FindClass
    __android_log_print(ANDROID_LOG_VERBOSE, "__GUI__", "****** got class from obj *****" );
   jmethodID mid = env->GetMethodID(cls, "jniTest2", "()I");
    __android_log_print(ANDROID_LOG_VERBOSE, "__GUI__", "****** got method id jniTest2() *****" );
   if(!mid) 
    return;  // method does not exist, should write some logs
    __android_log_print(ANDROID_LOG_VERBOSE, "__GUI__", "****** calling jniTest2() *****" );
   int i = env->CallIntMethod(obj, mid);
    __android_log_print(ANDROID_LOG_VERBOSE, "__GUI__", "****** back from jniTest2() *****" );

    __android_log_print(ANDROID_LOG_VERBOSE, "__GUI__", "****** create Java String *****" );
    jstring jstr = pEnv->NewStringUTF("This string comes from JNI");
    

    __android_log_print(ANDROID_LOG_VERBOSE, "__GUI__", "****** get Java method messageMe(string) *****" );
    jmethodID messageMe = pEnv->GetMethodID(cls, "messageMe", "(Ljava/lang/String;)Ljava/lang/String;");
    if( messageMe == NULL ) {
        __android_log_print(ANDROID_LOG_VERBOSE, "__GUI__", "****** INVALID MethodID *****" );
    }

    __android_log_print(ANDROID_LOG_VERBOSE, "__GUI__", "****** calling messageMe( jstr ) *****" );
    jstring rv = (jstring)env->CallObjectMethod(obj, messageMe, jstr );
    __android_log_print(ANDROID_LOG_VERBOSE, "__GUI__", "****** back from messageMe( jstr ) *****" );
    
    const char *strReturn = env->GetStringUTFChars( rv, 0);
    __android_log_print(ANDROID_LOG_VERBOSE, "__GUI__", "****** %s", strReturn );
    env->ReleaseStringUTFChars(rv, strReturn);
    
    
}

extern "C" void Java_com_jimmysoftware_clearscreen_SDLActivity_nativeSetAssets(JNIEnv* env, jclass cls, jobject assetManager) {
    __android_log_print(ANDROID_LOG_VERBOSE, "__GUI__", "****** save pAssetManager, pEnv, jni_cls *****" );
    pAssetManager = AAssetManager_fromJava(env, assetManager);
    pEnv = env;
    jni_cls = cls;
    
    __android_log_print(ANDROID_LOG_VERBOSE, "__GUI__", "****** get Java Class *****" );
    jclass clazz = pEnv->FindClass("com/jimmysoftware/pes/SDLActivity");
    if( clazz == NULL ) {
        __android_log_print(ANDROID_LOG_VERBOSE, "__GUI__", "****** INVALID CLASS *****" );
    }
    
    
    __android_log_print(ANDROID_LOG_VERBOSE, "__GUI__", "****** get Java static method jniTest() *****" );
    jmethodID jniTest = pEnv->GetStaticMethodID(cls, "jniTest", "()V");
    if( jniTest == NULL ) {
        __android_log_print(ANDROID_LOG_VERBOSE, "__GUI__", "****** INVALID jniTest *****" );
    }
    
    __android_log_print(ANDROID_LOG_VERBOSE, "__GUI__", "****** call static jniTest() *****" );
    pEnv->CallStaticVoidMethod( cls, jniTest );
    __android_log_print(ANDROID_LOG_VERBOSE, "__GUI__", "****** back to JNI safely *****" );
}

extern "C" void Java_com_jimmysoftware_clearscreen_SDLActivity_nativeSetupJNI(JNIEnv* env, jclass cls) {
    Java_org_libsdl_app_SDLActivity_nativeSetupJNI(env, cls);
}

extern "C" void Java_com_jimmysoftware_clearscreen_SDLAudioManager_nativeSetupJNI(JNIEnv* env, jclass cls) {
    Java_org_libsdl_app_SDLAudioManager_nativeSetupJNI(env, cls);
}

extern "C" void Java_com_jimmysoftware_clearscreen_SDLControllerManager_nativeSetupJNI(JNIEnv* env, jclass cls) {
    Java_org_libsdl_app_SDLControllerManager_nativeSetupJNI(env, cls);
}


extern "C" void Java_com_jimmysoftware_clearscreen_SDLActivity_nativeLowMemory(JNIEnv* env, jclass cls) {
    Java_org_libsdl_app_SDLActivity_nativeLowMemory(env, cls);
}

extern "C" void Java_com_jimmysoftware_clearscreen_SDLActivity_nativeQuit(JNIEnv* env, jclass cls) {
    Java_org_libsdl_app_SDLActivity_nativeQuit(env, cls);
}

extern "C" void Java_com_jimmysoftware_clearscreen_SDLActivity_nativePause(JNIEnv* env, jclass cls) {
    Java_org_libsdl_app_SDLActivity_nativePause(env, cls);
}

extern "C" void Java_com_jimmysoftware_clearscreen_SDLActivity_nativeResume(JNIEnv* env, jclass cls) {
    Java_org_libsdl_app_SDLActivity_nativeResume(env, cls);
}

extern "C" void Java_com_jimmysoftware_clearscreen_SDLActivity_onNativeDropFile(JNIEnv* env, jclass jcls, jstring filename) {
    Java_org_libsdl_app_SDLActivity_onNativeDropFile(env, jcls, filename);
}

extern "C" void Java_com_jimmysoftware_clearscreen_SDLActivity_onNativeResize(JNIEnv* env, jclass jcls, jint width, jint height, jint format, jfloat rate) {
    Java_org_libsdl_app_SDLActivity_onNativeResize(env, jcls, width, height, format, rate);
}

extern "C" jint Java_com_jimmysoftware_clearscreen_SDLControllerManager_onNativePadDown(JNIEnv* env, jclass jcls, jint device_id, jint keycode) {
    return Java_org_libsdl_app_SDLControllerManager_onNativePadDown(env, jcls, device_id, keycode);
}

extern "C" jint Java_com_jimmysoftware_clearscreen_SDLControllerManager_onNativePadUp(JNIEnv* env, jclass jcls, jint device_id, jint keycode) {
    return Java_org_libsdl_app_SDLControllerManager_onNativePadUp(env, jcls, device_id, keycode);
}

extern "C" void Java_com_jimmysoftware_clearscreen_SDLControllerManager_onNativeJoy(JNIEnv* env, jclass jcls, jint device_id, jint axis, jfloat value) {
    Java_org_libsdl_app_SDLControllerManager_onNativeJoy( env,  jcls,  device_id,  axis, value);
}

extern "C" void Java_com_jimmysoftware_clearscreen_SDLControllerManager_onNativeHat(JNIEnv* env, jclass jcls, jint device_id, jint hat_id, jint x, jint y) {
    Java_org_libsdl_app_SDLControllerManager_onNativeHat(env, jcls, device_id, hat_id, x, y);
}

extern "C" void Java_com_jimmysoftware_clearscreen_SDLActivity_onNativeKeyDown(JNIEnv* env, jclass jcls, jint keycode) {
    Java_org_libsdl_app_SDLActivity_onNativeKeyDown(env, jcls, keycode);
}

extern "C" void Java_com_jimmysoftware_clearscreen_SDLActivity_onNativeKeyUp(JNIEnv* env, jclass jcls, jint keycode) {
    Java_org_libsdl_app_SDLActivity_onNativeKeyUp(env, jcls, keycode);
}

extern "C" void Java_com_jimmysoftware_clearscreen_SDLActivity_onNativeKeyboardFocusLost(JNIEnv* env, jclass jcls) {
    Java_org_libsdl_app_SDLActivity_onNativeKeyboardFocusLost( env, jcls);
}

extern "C" void Java_com_jimmysoftware_clearscreen_SDLActivity_onNativeMouse(JNIEnv* env, jclass jcls, jint button, jint action, jfloat x, jfloat y) {
    Java_org_libsdl_app_SDLActivity_onNativeMouse(env, jcls, button, action, x, y);
}

extern "C" void Java_com_jimmysoftware_clearscreen_SDLActivity_onNativeTouch(JNIEnv* env, jclass jcls, jint touch_device_id_in, jint pointer_finger_id_in, jint action, jfloat x, jfloat y, jfloat p) {
    Java_org_libsdl_app_SDLActivity_onNativeTouch(env, jcls, touch_device_id_in, pointer_finger_id_in, action, x, y, p);
}

extern "C" void Java_com_jimmysoftware_clearscreen_SDLActivity_onNativeAccel(JNIEnv* env, jclass jcls, jfloat x, jfloat y, jfloat z) {
    Java_org_libsdl_app_SDLActivity_onNativeAccel(env, jcls, x, y, z);
}

extern "C" void Java_com_jimmysoftware_clearscreen_SDLActivity_onNativeSurfaceChanged(JNIEnv* env, jclass jcls) {
    Java_org_libsdl_app_SDLActivity_onNativeSurfaceChanged(env, jcls);
}

extern "C" void Java_com_jimmysoftware_clearscreen_SDLActivity_onNativeSurfaceDestroyed(JNIEnv* env, jclass jcls) {
    Java_org_libsdl_app_SDLActivity_onNativeSurfaceDestroyed(env, jcls);
}

extern "C" jint Java_com_jimmysoftware_clearscreen_SDLControllerManager_nativeAddJoystick(JNIEnv* env, jclass jcls, jint device_id, jstring device_name, jint is_accelerometer, jint nbuttons, jint naxes, jint nhats, jint nballs) {
    return Java_org_libsdl_app_SDLControllerManager_nativeAddJoystick(env, jcls, device_id, device_name, is_accelerometer, nbuttons, naxes, nhats, nballs);
}

extern "C" jint Java_com_jimmysoftware_clearscreen_SDLControllerManager_nativeRemoveJoystick(JNIEnv* env, jclass jcls, jint device_id) {
    return Java_org_libsdl_app_SDLControllerManager_nativeRemoveJoystick(env,  jcls, device_id);
}

extern "C" jstring Java_com_jimmysoftware_clearscreen_SDLActivity_nativeGetHint(JNIEnv* env, jclass cls, jstring name) {
    return Java_org_libsdl_app_SDLActivity_nativeGetHint(env, cls, name);
}

extern "C" void Java_com_jimmysoftware_clearscreen_SDLInputConnection_nativeCommitText(JNIEnv* env, jclass cls, jstring text, jint newCursorPosition) {
    Java_org_libsdl_app_SDLInputConnection_nativeCommitText(env, cls, text, newCursorPosition);
}

extern "C" void Java_com_jimmysoftware_clearscreen_SDLInputConnection_nativeSetComposingText(JNIEnv* env, jclass cls, jstring text, jint newCursorPosition) {
    Java_org_libsdl_app_SDLInputConnection_nativeSetComposingText(env, cls, text, newCursorPosition);
}
