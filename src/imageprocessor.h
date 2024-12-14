#ifndef IMAGEPROCESSOR_H
#define IMAGEPROCESSOR_H

#include <opencv2/opencv.hpp>
#include <vector>
#include <QString>
#include <QDebug>

class ImageProcessor {
public:
    ImageProcessor() {}

    std::vector<cv::Mat> loadImages(const QString& path, int numImages) {
        std::vector<cv::Mat> images;
        for (int i = 0; i < numImages; ++i) {
            QString filename = path + "/image_" + QString::number(i) + ".jpg";
            cv::Mat img = cv::imread(filename.toStdString());
            if (!img.empty()) {
                images.push_back(img);
            } else {
                qDebug() << "Ошибка загрузки изображения:" << filename;
            }
        }
        return images;
    }

    int labelImage(const QString& filename) {
        return filename.contains("banana") ? 1 : 0;
    }
};

#endif // IMAGEPROCESSOR_H