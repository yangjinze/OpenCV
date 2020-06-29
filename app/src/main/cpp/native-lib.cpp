#include <jni.h>
#include <string>
//#include <../jniLibs/include/opencv2/opencv.hpp> TODO 指明全路径?
#include <opencv2/opencv.hpp>
#include <android/bitmap.h> //jni对bitmap操作相关接口,AndroidBitmapInfo等
#include <android/log.h> //控制台log输出

#define TAG "JNI_TAG"
#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG,TAG,__VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR,TAG,__VA_ARGS__)

using namespace cv; // TODO 这个对opencv的作用是啥, 下面可以直接声明Mat，而不是CvMat

extern "C" {
JNIEXPORT void JNICALL Java_com_yang_opencv_MainActivity_toGrayBitmap(JNIEnv *env, jobject thiz, jobject bitmap);
Mat bitMapToMat(JNIEnv* jniEnv, jobject bitmap);
Mat toGrayMat(Mat mat);
void matToBitMap(JNIEnv* jniEnv, Mat mat, jobject bitmap);
}


extern "C"
JNIEXPORT void JNICALL
Java_com_yang_opencv_MainActivity_toGrayBitmap(JNIEnv *env, jobject thiz, jobject bitmap) {
    LOGD("bitMapToMat, start");
    Mat mat = bitMapToMat(env, bitmap);
    LOGD("toGrayMat, start");
    Mat grayMat = toGrayMat(mat);
    LOGD("matToBitMap, start");
    matToBitMap(env, grayMat, bitmap);
}

/**
 * bitmap对象转Mat矩阵，c++中对象Mat就是一个矩阵
 */
Mat bitMapToMat(JNIEnv* jniEnv, jobject bitmap) {//TODO 开头加了private声明就存在问题？？？
    //1.获取bitmap信息,存放在bitmapInfo中
    AndroidBitmapInfo bitmapInfo;
    AndroidBitmap_getInfo(jniEnv, bitmap, &bitmapInfo);

    //2.先锁定画布
    void* pixelsPtr; //存放bitmap首地址
    AndroidBitmap_lockPixels(jniEnv, bitmap, &pixelsPtr);
    //3.根据bitmap信息转换mat
    Mat mat;
    //android上bitmap 格式与Mat的type存在对应关系
    switch (bitmapInfo.format) {
        case ANDROID_BITMAP_FORMAT_RGBA_8888:
            LOGD("bitMapToMat, rgba---> cv_8uc4");
            mat = Mat(bitmapInfo.height, bitmapInfo.width, CV_8UC4, pixelsPtr);
            break;
        case ANDROID_BITMAP_FORMAT_RGB_565:
            LOGD("bitMapToMat, rbg565---> cv_8uc4");
            mat = Mat(bitmapInfo.height, bitmapInfo.width, CV_8UC2, pixelsPtr);
            break;
        default: //TODO 其它对应关系呢？ 其它情况怎么转换
            LOGD("bitMapToMat, default---> cv_8uc4");
            mat = Mat(bitmapInfo.height, bitmapInfo.width, CV_8UC2, pixelsPtr);
            break;
    }
    //4.解锁画布
    AndroidBitmap_unlockPixels(jniEnv,bitmap);
    return mat;
}
//void bitMapToMat(JNIEnv* jniEnv, jobject bitmap, Mat &mat) {//TODO mat入参是引用还是指针？
//    //1.获取bitmap信息,存放在bitmapInfo中
//    AndroidBitmapInfo bitmapInfo;
//    AndroidBitmap_getInfo(jniEnv, bitmap, &bitmapInfo);
//
//    //2.先锁定画布
//    void* pixelsPtr; //存放bitmap首地址
//    AndroidBitmap_lockPixels(jniEnv, bitmap, &pixelsPtr);
//    //3.根据bitmap信息转换mat
//    Mat tempMat;
//    //android上bitmap 格式与Mat的type存在对应关系
//    switch (bitmapInfo.format) {
//        case ANDROID_BITMAP_FORMAT_RGBA_8888:
//            LOGD("bitMapToMat, rgba---> cv_8uc4");
//            tempMat = Mat(bitmapInfo.height, bitmapInfo.width, CV_8UC4, pixelsPtr);
//            break;
//        case ANDROID_BITMAP_FORMAT_RGB_565:
//            LOGD("bitMapToMat, rbg565---> cv_8uc4");
//            tempMat = Mat(bitmapInfo.height, bitmapInfo.width, CV_8UC2, pixelsPtr);
//            break;
//        default: //TODO 其它对应关系呢？ 其它情况怎么转换
//            LOGD("bitMapToMat, default---> cv_8uc4");
//            tempMat = Mat(bitmapInfo.height, bitmapInfo.width, CV_8UC2, pixelsPtr);
//            break;
//    }
//    //4.解锁画布
//    AndroidBitmap_unlockPixels(jniEnv,bitmap);
//}

void matToBitMap(JNIEnv* jniEnv, Mat mat, jobject bitmap) {
    //1.获取bitmap信息,存放在bitmapInfo中
    AndroidBitmapInfo bitmapInfo;
    AndroidBitmap_getInfo(jniEnv, bitmap, &bitmapInfo);

    //2.先锁定画布
    void* pixelsPtr; //存放bitmap首地址
    AndroidBitmap_lockPixels(jniEnv, bitmap, &pixelsPtr);

    if(bitmapInfo.format == ANDROID_BITMAP_FORMAT_RGBA_8888){// C4
        Mat temp(bitmapInfo.height,bitmapInfo.width,CV_8UC4,pixelsPtr);
        if(mat.type() == CV_8UC4){
            mat.copyTo(temp);
        }
        else if(mat.type() == CV_8UC2){
            cvtColor(mat,temp,COLOR_BGR5652BGRA);
        }
        else if(mat.type() == CV_8UC1){// 灰度 mat
            cvtColor(mat,temp,COLOR_GRAY2BGRA);
        }
    } else if(bitmapInfo.format == ANDROID_BITMAP_FORMAT_RGB_565){// C2
        Mat temp(bitmapInfo.height,bitmapInfo.width,CV_8UC2,pixelsPtr);
        if(mat.type() == CV_8UC4){
            cvtColor(mat,temp,COLOR_BGRA2BGR565);
        }
        else if(mat.type() == CV_8UC2){
            mat.copyTo(temp);

        }
        else if(mat.type() == CV_8UC1){// 灰度 mat
            cvtColor(mat,temp,COLOR_GRAY2BGR565);
        }
    }
    // 其他要自己去转

    // 解锁 Bitmap 画布
    AndroidBitmap_unlockPixels(jniEnv,bitmap);
}
/**
 * 转换为灰度图
 *
 * @param mat
 */
Mat toGrayMat(Mat mat) {
    Mat  grayMat;
    cvtColor(mat, grayMat, COLOR_BGRA2GRAY);
    return grayMat;
}



