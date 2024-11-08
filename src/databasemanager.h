#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QJsonDocument>
#include <QJsonObject>

class DatabaseManager {
public:
    DatabaseManager(const QString& dbName, const QString& user, const QString& password, const QString& host, int port) {
        db = QSqlDatabase::addDatabase("QPSQL");
        db.setHostName(host);
        db.setDatabaseName(dbName);
        db.setUserName(user);
        db.setPassword(password);
        db.setPort(port);

        if (!db.open()) {
            qDebug() << "Ошибка подключения к БД:" << db.lastError().text();
        } else {
            qDebug() << "Подключение к БД успешно.";
        }
    }

    ~DatabaseManager() {
        if (db.isOpen()) {
            db.close();
        }
    }

    bool saveModel(const QJsonDocument& modelJson) {
        QByteArray compressedData = qCompress(modelJson.toJson(QJsonDocument::Compact), 9);
        QSqlQuery query;
        query.prepare("INSERT INTO models (model_data) VALUES (:model)");
        query.bindValue(":model", compressedData.toBase64()); 

        if (!query.exec()) {
            qDebug() << "Ошибка вставки в БД:" << query.lastError().text();
            return false;
        }
        return true;
    }

    QJsonDocument loadModel(int modelId) {
        QSqlQuery query;
        query.prepare("SELECT model_data FROM models WHERE id = :id");
        query.bindValue(":id", modelId);

        if (!query.exec() || !query.next()) {
            qDebug() << "Ошибка загрузки модели из БД:" << query.lastError().text();
            return QJsonDocument();
        }

        QByteArray compressedData = QByteArray::fromBase64(query.value(0).toByteArray());
        QByteArray modelData = qUncompress(compressedData);

        return QJsonDocument::fromJson(modelData);
    }



private:
    QSqlDatabase db;
};

#endif // DATABASEMANAGER_H
