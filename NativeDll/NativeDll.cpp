#include "pch.h"
#include <cstdlib>

// 由于jvm和c++对中文的编码不一样，因此需要转码。 utf8/16转换成gb2312
char* jstringToChar(JNIEnv* env, jstring jstr) {
    int length = (env)->GetStringLength(jstr);
    const jchar* jcstr = (env)->GetStringChars(jstr, 0);
    char* rtn = (char*)malloc(length * 2 + 1);
    int size = 0;
    size = WideCharToMultiByte(CP_ACP, 0, (LPCWSTR)jcstr, length, rtn,
        (length * 2 + 1), NULL, NULL);
    if (size <= 0)
        return NULL;
    (env)->ReleaseStringChars(jstr, jcstr);
    rtn[size] = 0;
    return rtn;
}

// 由于jvm和c++对中文的编码不一样，因此需要转码。gb2312转换成utf8/16
jstring charTojstring(JNIEnv* env, const char* str) {
    jstring rtn = 0;
    int slen = strlen(str);
    unsigned short* buffer = 0;
    if (slen == 0)
        rtn = (env)->NewStringUTF(str);
    else {
        int length = MultiByteToWideChar(CP_ACP, 0, (LPCSTR)str, slen, NULL, 0);
        buffer = (unsigned short*)malloc(length * 2 + 1);
        if (MultiByteToWideChar(CP_ACP, 0, (LPCSTR)str, slen, (LPWSTR)buffer, length) > 0)
            rtn = (env)->NewString((jchar*)buffer, length);
        // 释放内存
        free(buffer);
    }
    return rtn;
}


JNIEXPORT void JNICALL Java_com_sixsixdog_JNI_DllPrint_TestPrint
(JNIEnv*, jobject)
{
	printf("123");
}

JNIEXPORT jint JNICALL Java_com_sixsixdog_JNI_DllPrint_TestMessageBox
(JNIEnv* env, jobject, jstring msg)
{
    auto cMsg = jstringToChar(env, msg);
    return MessageBoxA(0, cMsg, "ok", MB_OK);
}
