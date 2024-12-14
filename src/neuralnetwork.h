#ifndef NEURALNETWORK_H
#define NEURALNETWORK_H

#include <opencv2/opencv.hpp>
#include <opencv2/ml.hpp>
#include <QByteArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QDebug>

class NeuralNetwork {
public:
    NeuralNetwork() {
        mlp = cv::ml::ANN_MLP::create();
    }

    void train(const std::vector<cv::Mat>& images, const std::vector<int>& labels) {
        cv::Mat trainData, trainLabels;

        for (size_t i = 0; i < images.size(); ++i) {
            cv::Mat img;
            cv::resize(images[i], img, cv::Size(32, 32));  
            img = img.reshape(1, 1);  
            trainData.push_back(img);
            trainLabels.push_back(labels[i]);
        }

        trainData.convertTo(trainData, CV_32F);

        cv::Mat layers = (cv::Mat_<int>(3, 1) << 1024, 64, 2);
        mlp->setLayerSizes(layers);
        mlp->setActivationFunction(cv::ml::ANN_MLP::SIGMOID_SYM);

        cv::Ptr<cv::ml::TrainData> tData = cv::ml::TrainData::create(trainData, cv::ml::ROW_SAMPLE, trainLabels);
        mlp->train(tData);
    }

    int predict(const cv::Mat& image) {
        cv::Mat img;
        cv::resize(image, img, cv::Size(32, 32));
        img = img.reshape(1, 1);
        img.convertTo(img, CV_32F);

        cv::Mat response;
        mlp->predict(img, response);
        return response.at<float>(0, 0) > 0.5 ? 1 : 0;  
    }

    QString serializeModel() {
        QJsonObject modelJson;
        modelJson["weights"] = "serialized_weights_placeholder";  // Сериализация данных модели (заглушка)
        QJsonDocument doc(modelJson);
        return QString(doc.toJson(QJsonDocument::Compact));
    }

    void deserializeModel(const QString& modelData) {
        QJsonDocument doc = QJsonDocument::fromJson(modelData.toUtf8());
        if (!doc.isObject()) {
            qDebug() << "Ошибка: данные модели не в формате JSON.";
            return;
        }

        QJsonObject modelJson = doc.object();
        QString weights = modelJson["weights"].toString();
        qDebug() << "Десериализованные веса:" << weights;  // Десериализация данных модели (заглушка)
    }

private:
    cv::Ptr<cv::ml::ANN_MLP> mlp;
};

#endif // NEURALNETWORK_H
