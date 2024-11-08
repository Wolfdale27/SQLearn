#include <QApplication>
#include <QByteArray>
#include "databasemanager.h"
#include "dnnmodel.h"
#include "imageprocessor.h"
#include "camerarecognition.h"

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);

    DatabaseManager dbManager("zephyr27", "wolfdale27", "4005", "localhost", 5432);

    ImageProcessor imgProcessor;
    std::vector<cv::Mat> images = imgProcessor.loadImages("/home/wolfdale/bananas", 15);
    std::vector<int> labels = {0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0};

    DNNModel dnnModel;
    dnnModel.train(images, labels);

    QJsonDocument modelJson = dnnModel.serializeModel();
    if (dbManager.saveModel(modelJson)) {
        qDebug() << "Модель успешно сохранена в БД.";
    }

    QJsonDocument loadedModelJson = dbManager.loadModel(1);
    dnnModel.deserializeModel(loadedModelJson);

    CameraRecognition cameraRecognition(dnnModel);
    cameraRecognition.startRecognition();

    return a.exec();
}
