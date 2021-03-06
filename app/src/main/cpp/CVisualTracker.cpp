//
// Created by GP62-7RE on 2018/1/8.
//
#include <jni.h>
#include <opencv2/opencv.hpp>

using namespace cv;

extern "C"{
JNIEXPORT jintArray JNICALL Java_martianlee_trackingapplication_CVisualTracker_doTemplateMatch(
        JNIEnv *env, jobject obj, jlong FrameAddr, jlong m_pROIAddr, jlong resultAddr, jint Method){
    cv::Mat& Frame = *(cv::Mat*) FrameAddr;
    cv::Mat& m_pROI = *(cv::Mat*) m_pROIAddr;
    cv::Mat& resultMat = *(cv::Mat*) resultAddr;

    cv::Point MaxPoint;
    cv::Point MinPoint;
    double MaxVal;
    double MinVal;

    int *resultPoint = new int(4);

    cv::matchTemplate(Frame, m_pROI, resultMat, Method);

    cv::minMaxLoc(resultMat, &MinVal, &MaxVal, &MinPoint, &MaxPoint, cv::noArray());

    resultPoint[0] = MinPoint.x;
    resultPoint[1] = MinPoint.y;
    resultPoint[2] = MaxPoint.x;
    resultPoint[3] = MaxPoint.y;

    jintArray result = env->NewIntArray(4);
    env->SetIntArrayRegion(result, 0, 4, resultPoint);

    return result;
}
}

extern "C" {
JNIEXPORT jintArray JNICALL Java_martianlee_trackingapplication_CVisualTracker_doMeanShift(
        JNIEnv *env, jobject obj, jlong m_backprojAddr, jint x, jint y, jint width, jint height) {
    cv::Mat &m_backproj = *(cv::Mat *) m_backprojAddr;
    cv::Rect TrackRect;

    int *resultRect = new int(4);

    TrackRect.x = x;
    TrackRect.y = y;
    TrackRect.width = width;
    TrackRect.height = height;

    TermCriteria termCriteria(cv::TermCriteria::EPS, 10, 1.0);

    cv::meanShift(m_backproj, TrackRect, termCriteria);

    resultRect[0] = TrackRect.x;
    resultRect[1] = TrackRect.y;
    resultRect[2] = TrackRect.width;
    resultRect[3] = TrackRect.height;

    jintArray result = env->NewIntArray(4);
    env->SetIntArrayRegion(result, 0, 4, resultRect);

    return result;
}
}

extern "C" {
JNIEXPORT jdoubleArray JNICALL Java_martianlee_trackingapplication_CVisualTracker_doCamShift(
        JNIEnv *env, jobject obj, jlong m_backprojAddr, jint x, jint y, jint width, jint height) {
    cv::Mat &m_backproj = *(cv::Mat *) m_backprojAddr;
    cv::Rect TrackRect;

    double *resultRect = new double(5);

    TrackRect.x = x;
    TrackRect.y = y;
    TrackRect.width = width;
    TrackRect.height = height;

    TermCriteria termCriteria(cv::TermCriteria::EPS, 10, 1.0);

    cv::RotatedRect trackRect = cv::CamShift(m_backproj, TrackRect, termCriteria);

    resultRect[0] = trackRect.angle;
    resultRect[1] = trackRect.center.x;
    resultRect[2] = trackRect.center.y;
    resultRect[3] = trackRect.size.width;
    resultRect[4] = trackRect.size.height;

    jdoubleArray result;

    result = env->NewDoubleArray(5);
    env->SetDoubleArrayRegion(result, 0, 5, resultRect);

    return result;
}
}