/*
 * Copyright (C) 2023 Elytrium
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <jni/net_elytrium_rnnoise_RnnNative.h>
#include <rnnoise.h>

JNIEXPORT jint JNICALL Java_net_elytrium_rnnoise_RnnNative_getSize(JNIEnv *env, jclass class) {
  return (jint) rnnoise_get_size();
}

JNIEXPORT jint JNICALL Java_net_elytrium_rnnoise_RnnNative_getFrameSize(JNIEnv *env, jclass class) {
  return (jint) rnnoise_get_frame_size();
}

JNIEXPORT jlong JNICALL Java_net_elytrium_rnnoise_RnnNative_create(JNIEnv *env, jclass class, jlong jmodel) {
  return (jlong) rnnoise_create((RNNModel*) jmodel);
}

JNIEXPORT void JNICALL Java_net_elytrium_rnnoise_RnnNative_destroy(JNIEnv *env, jclass class, jlong jstate) {
  rnnoise_destroy((DenoiseState*) jstate);
}

JNIEXPORT void JNICALL Java_net_elytrium_rnnoise_RnnNative_processFrameBuffer(JNIEnv *env, jclass class, jlong jstate, jobject jout, jobject jin) {
  char *in = (*env)->GetDirectBufferAddress(env, jin);
  char *out = (*env)->GetDirectBufferAddress(env, jout);
  rnnoise_process_frame((DenoiseState*) jstate, (float*) in, (float*) out);
}

JNIEXPORT void JNICALL Java_net_elytrium_rnnoise_RnnNative_processFrame(JNIEnv *env, jclass class, jlong jstate, jfloatArray jout, jfloatArray jin) {
  float *in = (*env)->GetFloatArrayElements(env, jin, 0);
  float *out = (*env)->GetFloatArrayElements(env, jout, 0);
  rnnoise_process_frame((DenoiseState*) jstate, in, out);
  (*env)->ReleaseFloatArrayElements(env, jin, in, 0);
  (*env)->ReleaseFloatArrayElements(env, jout, out, 0);
}

JNIEXPORT jlong JNICALL Java_net_elytrium_rnnoise_RnnNative_modelFromFile(JNIEnv *env, jclass class, jstring jmodel) {
  const char *model = (*env)->GetStringUTFChars(env, jmodel, NULL);
  FILE* model_file = fopen(model, "rb");
  jlong model_pointer = (jlong) rnnoise_model_from_file(model_file);
  fclose(model_file);
  (*env)->ReleaseStringUTFChars(env, jmodel, model);
  return model_pointer;
}

JNIEXPORT void JNICALL Java_net_elytrium_rnnoise_RnnNative_modelFree(JNIEnv *env, jclass class, jlong jstate) {
  rnnoise_model_free((RNNModel*) jstate);
}
