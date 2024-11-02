#ifndef CAMERALISTMANAGER_H
#define CAMERALISTMANAGER_H

#include <QObject>

class CameraListManager : public QObject
{
    Q_OBJECT
public:
    explicit CameraListManager(QObject *parent = nullptr);

signals:

};

#endif // CAMERALISTMANAGER_H
