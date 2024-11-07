#ifndef CAMERALISTMANAGER_H
#define CAMERALISTMANAGER_H

#include <QObject>

class QNetworkAccessManager;
class QNetworkReply;

struct TrafficCamera
{
    QString livestreamUrl;
};

QVector<TrafficCamera> cameraListFromJson(const QJsonDocument& jsonDocument);

class CameraListManager : public QObject
{
    Q_OBJECT
public:
    explicit CameraListManager(QObject *parent = nullptr);

    void loadCameraList();

signals:
     void cameraListChanged(QVector<TrafficCamera>);

private slots:
    void onNetworkReplyReceived(QNetworkReply *reply);

private:
    QNetworkAccessManager *m_networkManager;

};

#endif // CAMERALISTMANAGER_H
