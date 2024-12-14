#ifndef CAMERARECOGNITION_H
#define CAMERARECOGNITION_H

#include <opencv2/opencv.hpp>
#include <QDebug>
#include "neuralnetwork.h"

class CameraRecognition {
public:
    CameraRecognition(NeuralNetwork& network) : nn(network) {}

    void startRecognition() {
        cv::VideoCapture cap(0);

        if (!cap.isOpened()) {
            qDebug() << "Ошибка открытия камеры!";
            return;
        }

        cv::Mat frame;
        while (cap.read(frame)) {
            int label = nn.predict(frame);

            if (label == 1) {
                highlightObject(frame);
                qDebug() << "Распознанный объект:" << m_primaryObject;
            } else {
                qDebug() << "Распознанный объект: Не " << m_primaryObject;
            }

            cv::imshow("Camera", frame);
            if (cv::waitKey(30) >= 0) break;  
        }
    }

private:
    NeuralNetwork& nn;
    QString m_primaryObject{"Банан"};

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
