
#include <jni.h>
#include <errno.h>
#include <sys/stat.h>
#include <android/asset_manager_jni.h>

#include "../../../src/game/ShootTest.h"
#include "../../../src/game/GameEngine.h"
#include "../../../src/game/GameManager.h"
#include "FileAPK.h"
#include "InputManager.h"

#ifndef _RETAIL_
#include <android/log.h>
#define LOGI(...) ((void)__android_log_print(ANDROID_LOG_INFO, "ShootTest", __VA_ARGS__))
#define LOGW(...) ((void)__android_log_print(ANDROID_LOG_WARN, "ShootTest", __VA_ARGS__))
#else
#define LOGI(...)
#define LOGW(...)
#endif

extern void RegisterObjects();
static double g_PreviousTime = 0;

// Initialize Audio
#include "ck/ck.h"
#include "ck/config.h"

/* return current time in seconds */
static double now_seconds(void)
{
    struct timespec res;
    clock_gettime(CLOCK_REALTIME, &res);
    return res.tv_sec + (double)res.tv_nsec/1e9;
}

extern "C"
{
    JNIEXPORT void JNICALL Java_com_amine_JNILib_init(JNIEnv * env, jobject obj, jobject activityObject, jobject assetManager, jstring internalPath, jstring externalPath);
    JNIEXPORT void JNICALL Java_com_amine_JNILib_step(JNIEnv * env, jobject obj);
	JNIEXPORT void JNICALL Java_com_amine_JNILib_setTouchState(JNIEnv * env, jobject obj, jfloat x, jfloat y, jboolean bPressed);
	JNIEXPORT void JNICALL Java_com_amine_JNILib_setBackPressed(JNIEnv * env, jobject obj, jboolean bPressed);
	JNIEXPORT void JNICALL Java_com_amine_JNILib_createContext(JNIEnv * env, jobject obj);
	JNIEXPORT void JNICALL Java_com_amine_JNILib_destroyContext(JNIEnv * env, jobject obj);
	JNIEXPORT void JNICALL Java_com_amine_JNILib_resizeScreen(JNIEnv * env, jobject obj, jint width, jint height);
	JNIEXPORT void JNICALL Java_com_amine_JNILib_pause(JNIEnv * env, jobject obj);
	JNIEXPORT void JNICALL Java_com_amine_JNILib_resume(JNIEnv * env, jobject obj);
	JNIEXPORT void JNICALL Java_com_amine_JNILib_destroy(JNIEnv * env, jobject obj);
	JNIEXPORT bool JNICALL Java_com_amine_JNILib_isExitRequested(JNIEnv * env, jobject obj);
	JNIEXPORT bool JNICALL Java_com_amine_JNILib_isBuyFullGameRequested(JNIEnv * env, jobject obj);
	JNIEXPORT void JNICALL Java_com_amine_JNILib_resetBuyFullGameRequest(JNIEnv * env, jobject obj);
	JNIEXPORT void JNICALL Java_com_amine_JNILib_setTrialMode(JNIEnv * env, jobject obj, jboolean bTrial);
};

JNIEXPORT void JNICALL Java_com_amine_JNILib_init(JNIEnv *env, jobject obj, jobject activityObject, jobject assetManager, jstring internalPath, jstring externalPath)
{
	LOGI("Java_com_amine_JNILib_init\n");
	AAssetManager* pAssetMgr = AAssetManager_fromJava(env, assetManager);	
	const char* strInternalPath = env->GetStringUTFChars(internalPath, NULL);
	const char* strExternalPath = env->GetStringUTFChars(externalPath, NULL);	
	shoot::FileAPK::SetAssetManager(pAssetMgr);	
	shoot::File::SetWritablePath(strInternalPath);
	shoot::File::SetExternalWritablePath(strExternalPath);
	
	LOGI("Initializing audio..\n");
	CkConfig config(env, activityObject);
	CkInit(&config);
	
#ifndef _RETAIL_
	// create external directory if it doesn't exist
	struct stat _stat;
	if(stat(strExternalPath, &_stat) == -1 && (errno == ENOENT))
	{
		std::string path = strExternalPath;
		std::string rootDir = path.substr(0, path.rfind("/"));
		LOGI("Creating directory: %s", rootDir.c_str());
		bool bError = mkdir(rootDir.c_str(), 0770) != 0;
		if(!bError)
		{
			LOGI("Creating directory: %s", strExternalPath);
			bError = mkdir(strExternalPath, 0770) != 0;
		}		
		if(bError)
		{
			LOGI("mkdir error: %s", strerror(errno));
		}
	}
#endif // _RETAIL_
}

JNIEXPORT void JNICALL Java_com_amine_JNILib_step(JNIEnv * env, jobject obj)
{	
	double currentTime = now_seconds();
	float fDeltaTime = currentTime-g_PreviousTime;
	g_PreviousTime = currentTime;
		
	shoot::InputManager::Instance()->Update();
	shoot::Engine::Instance()->Update(fDeltaTime);
	shoot::Engine::Instance()->Render();
}

JNIEXPORT void JNICALL Java_com_amine_JNILib_setTouchState(JNIEnv * env, jobject obj, jfloat x, jfloat y, jboolean bPressed)
{
	shoot::InputManager::TouchState state;
	state.vPosition = shoot::Vector2(x, y);
	state.bPressed = bPressed;
	shoot::InputManager::Instance()->SetTouchState(state);
}

JNIEXPORT void JNICALL Java_com_amine_JNILib_setBackPressed(JNIEnv * env, jobject obj, jboolean bPressed)
{
	shoot::InputManager::Instance()->SetKeyPressed(shoot::InputManager::KT_Back, bPressed);
}

JNIEXPORT void JNICALL Java_com_amine_JNILib_createContext(JNIEnv * env, jobject obj)
{
	shoot::Log << "createContext\n";
	if(shoot::Engine::Instance())	
	{
		shoot::Engine::Instance()->RequestResourceReload(&shoot::GameEngine::OnResourcesReloaded);
	}
}

JNIEXPORT void JNICALL Java_com_amine_JNILib_destroyContext(JNIEnv * env, jobject obj)
{
	shoot::Log << "destroyContext\n";
	if(shoot::Engine::Instance())
	{
		shoot::GraphicsDriver::Instance()->UnloadObjects();
	}
}

JNIEXPORT void JNICALL Java_com_amine_JNILib_resizeScreen(JNIEnv * env, jobject obj, jint width, jint height)
{
	shoot::Log << "resizeScreen\n";	
	// TODO: should start a native thread and do this in it.
	if(!shoot::Engine::Instance())
	{
		shoot::Engine* pEngine = new shoot::GameEngine(&RegisterObjects, shoot::GraphicsDriver::DT_OpenGLES_1_1, shoot::InputManager::IMT_Touch);
		pEngine->Init();
	}	
	shoot::Engine::Instance()->ResizeScreen(shoot::Size(width, height));	
}

JNIEXPORT void JNICALL Java_com_amine_JNILib_pause(JNIEnv * env, jobject obj)
{
	shoot::Log << "pause\n";
	if(shoot::Engine::Instance())
	{
		shoot::Engine::Instance()->OnPause();
	}
}

JNIEXPORT void JNICALL Java_com_amine_JNILib_resume(JNIEnv * env, jobject obj)
{
	shoot::Log << "resume\n";
	if(shoot::Engine::Instance())
	{
		shoot::Engine::Instance()->OnResume();
	}
}

JNIEXPORT void JNICALL Java_com_amine_JNILib_destroy(JNIEnv * env, jobject obj)
{
	shoot::Log << "destroy\n";
	if(shoot::Engine::Instance())
	{
		delete shoot::Engine::Instance();
	}
}

JNIEXPORT bool JNICALL Java_com_amine_JNILib_isExitRequested(JNIEnv * env, jobject obj)
{
	if(shoot::Engine::Instance())
	{
		return !shoot::Engine::Instance()->IsRunning();
	}
	return false;
}

JNIEXPORT bool JNICALL Java_com_amine_JNILib_isBuyFullGameRequested(JNIEnv * env, jobject obj)
{
	if(shoot::GameManager::Instance())
    {
		return shoot::GameManager::Instance()->HasGetFullGameRequest();
    }
	return false;
}

JNIEXPORT void JNICALL Java_com_amine_JNILib_resetBuyFullGameRequest(JNIEnv * env, jobject obj)
{
	if(shoot::GameManager::Instance())
    {
		shoot::GameManager::Instance()->ResetGetFullGameRequest();
    }
}

JNIEXPORT void JNICALL Java_com_amine_JNILib_setTrialMode(JNIEnv * env, jobject obj, jboolean bTrial)
{
	if(shoot::GameManager::Instance())
    {
		shoot::GameManager::Instance()->SetAndroidTrialMode(bTrial);
    }
}


