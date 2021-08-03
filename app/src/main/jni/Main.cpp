#include <list>
#include <vector>
#include <string.h>
#include <pthread.h>
#include <cstring>
#include <jni.h>
#include <unistd.h>
#include <fstream>
#include <iostream>
//#include <And64InlineHook/And64InlineHook.hpp>
#include "Includes/Vector2.h"
#include "Includes/Rect.h"
#include "Includes/Color.h"
#include "Includes/Quaternion.h"
#include "Includes/Logger.h"
#include "Includes/obfuscate.h"
#include "Includes/Utils.h"
#include "KittyMemory/MemoryPatch.h"
#include "Menu.h"

#if defined(__aarch64__)

#include <And64InlineHook/And64InlineHook.hpp>

#define HOOK(offset, ptr, orig) A64HookFunction((void *)getAbsoluteAddress(targetLibIL2CPP, string2Offset(OBFUSCATE_KEY(offset, 23479432523588))), (void *)ptr, (void **)&orig)

#else

#include <Substrate/CydiaSubstrate.h>

#define HOOK(offset, ptr, orig) MSHookFunction((void *)getAbsoluteAddress(targetLibIL2CPP, string2Offset(OBFUSCATE_KEY(offset, 23479432523588))), (void *)ptr, (void **)&orig)

#endif

void hooking(void *orig_fcn, void* new_fcn, void **orig_fcn_ptr)
{
#if defined(__aarch64__)
    A64HookFunction(orig_fcn, new_fcn, orig_fcn_ptr);
#else
    MSHookFunction(orig_fcn, new_fcn, orig_fcn_ptr);
#endif
}

//!!! KittyMemory || Hex-Patching !!!//
struct My_Patches {
    // let's assume we have patches for these functions for whatever game
    // like show in miniMap boolean function
    MemoryPatch GodMode, GodMode2, SliderExample;
    // etc...
} hexPatches;

bool isTest, isTest2;

//!!! Hooks !!!//

bool (*_method)(void *instance);
bool method(void *instance) {
    if (instance != NULL){
        if(isTest) {
            return true;
        }
    }
    return _method(instance);
}

int (*_method2)(void *instance);
int method2(void *instance){
    if (instance != NULL) {
        if(isTest2) {
            return 99;
        }
    }
    return _method2(instance);
}

void *hack_thread(void *) {
    LOGI(OBFUSCATE("pthread created"));

    do {
        sleep(1);
    } while (!isLibraryLoaded(targetLibIL2CPP));

    LOGI(OBFUSCATE("%s has been loaded"), (const char *) targetLibIL2CPP);

    //!!! A64HookFunction AKA ARM64, (DOES NOT WORK FOR SOME REASON!) !!!//

    //!!! This template supports ARMV-7 for now, if you know the solution, contribute or issue a page for ARM64 !!!//

    hexPatches.GodMode = MemoryPatch::createWithHex(targetLibIL2CPP, //Normal obfuscate
                                                    string2Offset(OBFUSCATE("0x000000")),
                                                    OBFUSCATE("00 00 A0 E3 1E FF 2F E1"));
    //You can also specify target lib like this
    hexPatches.GodMode2 = MemoryPatch::createWithHex("libil2cpp.so",
                                                     string2Offset(OBFUSCATE_KEY("0x000000", 23479432523588)), //64-bit key in decimal
                                                     OBFUSCATE_KEY("01 00 A0 E3 1E FF 2F E1", 0x3FE63DF21A3B)); //64-bit key in hex works too

    hooking((void*)getAbsoluteAddress(targetLibIL2CPP, string2Offset(OBFUSCATE_KEY("0x000000", 23479432523588))), (void *) method, (void **)&_method);

    HOOK("0x000000", method2, _method2);

    LOGI(OBFUSCATE("Done"));
    return NULL;
}

extern "C" {
JNIEXPORT jobjectArray
JNICALL
Java_com_simplefucker_source_ModMenu_getSettings(JNIEnv *env, jobject activityObject) {
    jobjectArray ret;

    const char *features[] = {
            OBFUSCATE("0_Category_First Color"),
            OBFUSCATE("-110_InputValue_First Color Red"),
            OBFUSCATE("-112_InputValue_First Color Green"),
            OBFUSCATE("-111_InputValue_First Color Blue"),
            OBFUSCATE("0_Category_Secondary Color"),
            OBFUSCATE("-210_InputValue_First Color Red"),
            OBFUSCATE("-212_InputValue_First Color Green"),
            OBFUSCATE("-211_InputValue_First Color Blue"),
    };

    int Total_Feature = (sizeof features /
                         sizeof features[0]); //Now you dont have to manually update the number everytime;
    ret = (jobjectArray)
            env->NewObjectArray(Total_Feature, env->FindClass(OBFUSCATE("java/lang/String")),
                                env->NewStringUTF(""));
    int i;
    for (i = 0; i < Total_Feature; i++)
        env->SetObjectArrayElement(ret, i, env->NewStringUTF(features[i]));

    return (ret);
}


JNIEXPORT jobjectArray
JNICALL
Java_com_simplefucker_source_ModMenu_getPlayer(JNIEnv *env, jobject activityObject) {
    jobjectArray ret;

    const char *features[] = {
            OBFUSCATE("0_Category_Player Category"),
            OBFUSCATE("100_Toggle_God Mode"),
            OBFUSCATE("151_SeekBar_Preview SeekBar_1_13131"),
    };

    int Total_Feature = (sizeof features /
                         sizeof features[0]); //Now you dont have to manually update the number everytime;
    ret = (jobjectArray)
            env->NewObjectArray(Total_Feature, env->FindClass(OBFUSCATE("java/lang/String")),
                                env->NewStringUTF(""));
    int i;
    for (i = 0; i < Total_Feature; i++)
        env->SetObjectArrayElement(ret, i, env->NewStringUTF(features[i]));

    pthread_t ptid;
    pthread_create(&ptid, NULL, antiLeech, NULL);

    return (ret);
}
JNIEXPORT jobjectArray
JNICALL
Java_com_simplefucker_source_ModMenu_getVisuals(JNIEnv *env, jobject activityObject) {
    jobjectArray ret;

    const char *features[] = {
            OBFUSCATE("0_Category_Weapon"),
            OBFUSCATE("101_Toggle_Ammo"),
            OBFUSCATE("151_SeekBar_Preview SeekBar_1_13131"),
    };

    int Total_Feature = (sizeof features /
                         sizeof features[0]); //Now you dont have to manually update the number everytime;
    ret = (jobjectArray)
            env->NewObjectArray(Total_Feature, env->FindClass(OBFUSCATE("java/lang/String")),
                                env->NewStringUTF(""));
    int i;
    for (i = 0; i < Total_Feature; i++)
        env->SetObjectArrayElement(ret, i, env->NewStringUTF(features[i]));

    pthread_t ptid;
    pthread_create(&ptid, NULL, antiLeech, NULL);

    return (ret);
}

JNIEXPORT void JNICALL
Java_com_simplefucker_source_SavedPrefs_Changes(JNIEnv *env, jclass clazz, jobject obj,
                                                jint feature, jint value, jboolean boolean, jstring str) {

    const char *featureName = env->GetStringUTFChars(str, 0);

    LOGD(OBFUSCATE("Feature name: %d - %s | Value: = %d | Bool: = %d"), feature, featureName, value,
         boolean);

    //!!! BE CAREFUL NOT TO ACCIDENTLY REMOVE break; !!!//

    switch (feature) {
        case 100:
            isTest = boolean;
            break;
        case 101:
            isTest2 = boolean;
            break;
    }
}
}



__attribute__((constructor))
void lib_main() {
    pthread_t ptid;
    pthread_create(&ptid, NULL, hack_thread, NULL);
}

