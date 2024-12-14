#include <QApplication>
#include "databasemanager.h"
#include "neuralnetwork.h"
#include "imageprocessor.h"
#include "camerarecognition.h"

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);

    DatabaseManager dbManager("models.db");

    ImageProcessor imgProcessor;
    std::vector<cv::Mat> images = imgProcessor.loadImages("/Users/wolfdale27/Downloads/bananas", 15);
    std::vector<int> labels = {0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0};

    NeuralNetwork neuralNetwork;
    neuralNetwork.train(images, labels);

    QString modelData = neuralNetwork.serializeModel();
    QJsonDocument modelJson = QJsonDocument::fromJson(modelData.toUtf8());
    if (dbManager.saveModel(modelJson)) {
        qDebug() << "Модель успешно сохранена в SQLite.";
    }

    QJsonDocument loadedModelJson = dbManager.loadModel(1);
    if (!loadedModelJson.isEmpty()) {
        QString loadedModelData = QString::fromUtf8(loadedModelJson.toJson());
        neuralNetwork.deserializeModel(loadedModelData);
        qDebug() << "Модель успешно загружена из SQLite.";
    }

    CameraRecognition cameraRecognition(neuralNetwork);
    cameraRecognition.startRecognition();

    return a.exec();
}
