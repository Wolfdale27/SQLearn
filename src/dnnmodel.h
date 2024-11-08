#ifndef DNNMODEL_H
#define DNNMODEL_H

#include <opencv2/opencv.hpp>
#include <opencv2/ml.hpp>
#include <QJsonDocument>
#include <QJsonObject>

class DNNModel {
public:
    DNNModel() {
        svm = cv::ml::SVM::create();
        svm->setType(cv::ml::SVM::C_SVC);
        svm->setKernel(cv::ml::SVM::LINEAR);
        svm->setTermCriteria(cv::TermCriteria(cv::TermCriteria::MAX_ITER, 100, 1e-6));
    }

    void train(const std::vector<cv::Mat>& images, const std::vector<int>& labels) {
        cv::Mat trainingData, labelsMat(labels);
        for (const auto& img : images) {
            cv::Mat resizedImg;
            cv::resize(img, resizedImg, cv::Size(224, 224));  
            cv::Mat flat;
            resizedImg.reshape(1, 1).convertTo(flat, CV_32F);  
            trainingData.push_back(flat);
        }
        svm->train(trainingData, cv::ml::ROW_SAMPLE, labelsMat);
    }


    int predict(const cv::Mat& image) {
    cv::Mat resizedImg;
    cv::resize(image, resizedImg, cv::Size(224, 224));
    cv::Mat flat;
    resizedImg.reshape(1, 1).convertTo(flat, CV_32F);

    return static_cast<int>(svm->predict(flat));
}



    QJsonDocument serializeModel() {
    QJsonObject modelJson;

    cv::FileStorage fs("", cv::FileStorage::WRITE | cv::FileStorage::MEMORY | cv::FileStorage::FORMAT_JSON);
    svm->write(fs); 
    std::string modelData = fs.releaseAndGetString();

    modelJson["model_data"] = QString::fromStdString(modelData);

    return QJsonDocument(modelJson);
}

    void deserializeModel(const QJsonDocument& modelJson) {
    if (!modelJson.isObject()) {
        qDebug() << "Ошибка: данные модели не в формате JSON-объекта.";
        return;
    }

    QString modelData = modelJson["model_data"].toString();
    cv::FileStorage fs(modelData.toStdString(), cv::FileStorage::READ | cv::FileStorage::MEMORY);
    if (!fs.isOpened()) {
        qDebug() << "Не удалось открыть данные модели для чтения.";
        return;
    }

    svm->read(fs.getFirstTopLevelNode());
}


private:
    cv::Ptr<cv::ml::SVM> svm;
};

#endif // DNNMODEL_H
