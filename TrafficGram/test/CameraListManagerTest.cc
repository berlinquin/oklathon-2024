#include <QFile>
#include <QFileInfo>
#include <QJsonDocument>
#include <gtest/gtest.h>

#include "../CameraListManager.h"

TEST(CameraListManagerTest, CameraPoleFromJson) {
    QFile cameraPolesFile("./data/CameraPoles.json");
    ASSERT_TRUE(cameraPolesFile.exists()) << "Rationale: test data should exist";

    cameraPolesFile.open(QIODeviceBase::ReadOnly);
    QByteArray cameraPolesData = cameraPolesFile.readAll();
    auto jsonDocument = QJsonDocument::fromJson(std::move(cameraPolesData));

    auto cameraPoles = cameraPolesFromJson(jsonDocument);
    EXPECT_FALSE(cameraPoles.empty());
}
