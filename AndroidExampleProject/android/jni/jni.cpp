#include <jni.h>
#include "../../common/JNIBridge.h"

extern "C" {

JNIEXPORT void JNICALL Java_com_example_spaceshipgame_GameLibJNIWrapper_on_1surface_1created
	(JNIEnv * env, jclass cls) {
		on_surface_created();
	}

JNIEXPORT void JNICALL Java_com_example_spaceshipgame_GameLibJNIWrapper_on_1surface_1changed
	(JNIEnv * env, jclass cls, jint width, jint height) {
		on_surface_changed(width,height);
	}


JNIEXPORT void JNICALL Java_com_example_spaceshipgame_GameLibJNIWrapper_on_1draw_1frame
	(JNIEnv * env, jclass cls) {
		on_draw_frame();
	}

}