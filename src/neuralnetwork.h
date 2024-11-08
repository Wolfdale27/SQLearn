#ifndef NEURALNETWORK_H
#define NEURALNETWORK_H

#include <opencv2/opencv.hpp>
#include <opencv2/ml.hpp>
#include <QByteArray>

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

    QByteArray serializeModel() {
        std::vector<uchar> buffer;
        cv::FileStorage fs(".json", cv::FileStorage::WRITE | cv::FileStorage::MEMORY);
        mlp->write(fs);
        std::string modelStr = fs.releaseAndGetString();
        QByteArray result = QByteArray::fromStdString(modelStr);
        return result;
    }

    void deserializeModel(const QByteArray& data) {
        cv::FileStorage fs(data.toStdString(), cv::FileStorage::READ | cv::FileStorage::MEMORY);
        mlp = cv::Algorithm::read<cv::ml::ANN_MLP>(fs.getFirstTopLevelNode());
    }

private:
    cv::Ptr<cv::ml::ANN_MLP> mlp;
};

#endif // NEURALNETWORK_H
