#ifndef CAMERARECOGNITION_H
#define CAMERARECOGNITION_H

#include <opencv2/opencv.hpp>
#include <QDebug>
#include "dnnmodel.h"

class CameraRecognition {
public:
    CameraRecognition(DNNModel& network) : dnnModel(network) {}

    void startRecognition() {
        cv::VideoCapture cap(0);

        if (!cap.isOpened()) {
            qDebug() << "Ошибка открытия камеры!";
            return;
        }

        cv::Mat frame;
        while (cap.read(frame)) {
            int label = dnnModel.predict(frame);

            if (label == 1) {
                highlightObject(frame);
                qDebug() << "Распознанный объект: Банан";
            } else {
                qDebug() << "Распознанный объект: Не банан";
            }

            cv::imshow("Camera", frame);
            if (cv::waitKey(30) >= 0) break;  
        }
    }

private:
    DNNModel& dnnModel;

    void highlightObject(cv::Mat& frame) {

        int x = frame.cols / 4;
        int y = frame.rows / 4;
        int width = frame.cols / 2;
        int height = frame.rows / 2;

        cv::rectangle(frame, cv::Rect(x, y, width, height), cv::Scalar(0, 255, 0), 2);

        cv::Mat objectMask = cv::Mat::zeros(frame.size(), CV_8UC3);
        cv::rectangle(objectMask, cv::Rect(x, y, width, height), cv::Scalar(0, 0, 255), cv::FILLED);

        double alpha = 0.3;  
        cv::addWeighted(objectMask, alpha, frame, 1 - alpha, 0, frame);
    }
};

#endif // CAMERARECOGNITION_H
