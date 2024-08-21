#ifndef GAME_LOG_H
#define GAME_LOG_H

#include <jni.h>
#include <android/log.h>
#include <iostream>
#include <streambuf>
#include <sstream>

class Log : public std::streambuf {

private:


public:
	static JNIEnv* env;
	static unsigned int count;

    Log() = default;

    static void w(const std::string& message, const char* file, int line) {
		std::ostringstream oss;
		oss << file << ":" << line << std::endl << message << std::endl << " ";
		__android_log_write(ANDROID_LOG_ERROR, "C++", oss.str().c_str());


		/*jclass clazz = env->FindClass("com/example/game/Log");
		jmethodID methodID = env->GetStaticMethodID(clazz, "e", "(Ljava/lang/String;Ljava/lang/String;)V");
		jstring jmessage1 = env->NewStringUTF("C++");
		jstring jmessage = env->NewStringUTF(oss.str().c_str());
		env->CallStaticVoidMethod(clazz, methodID, jmessage1, jmessage);
		env->DeleteLocalRef(jmessage1);
		env->DeleteLocalRef(jmessage);*/
    }



	static void log(const std::string& message, const char* file, int line) {
		if (count > 0) {
			std::ostringstream oss;
			oss << file << ":" << line << std::endl << message << std::endl << " ";
			__android_log_write(ANDROID_LOG_ERROR, "C++", oss.str().c_str());

			count--;
		}
	}



	template <typename T>
	static void reference(T& obj) {
		//std::reference_wrapper<T> refWrapper(obj);
		std::ostringstream oss;
		oss << "Reference: " << &obj;
		__android_log_write(ANDROID_LOG_INFO, "C++", oss.str().c_str());
	}




protected:
    int overflow(int c) override {
        if (c != EOF) {
            char buf[] = { static_cast<char>(c), '\0' };
            __android_log_write(ANDROID_LOG_ERROR, "C++", buf);
            //__android_log_print(ANDROID_LOG_ERROR, "C++", "%c", c);
        }
        return c;
    }

    virtual std::streamsize xsputn(const char* s, std::streamsize n) override {
        std::string str(s, n);
        __android_log_write(ANDROID_LOG_ERROR, "C++", str.c_str());
        return n;
    }

};

#define e(message) Log::w(message, __FILE__, __LINE__)
#define log(message) Log::log(message, __FILE__, __LINE__)

#endif //GAME_LOG_H
