#include <QFile>
#include <QFileInfo>
#include <QJsonDocument>
#include <gtest/gtest.h>

#include "../CameraListManager.h"

TEST(CameraListManagerTest, CameraPoleFromJson) {
    // GIVEN: A cached response from the CameraPoles API
    QFile cameraPolesFile("./data/CameraPoles.json");
    ASSERT_TRUE(cameraPolesFile.exists()) << "Rationale: test data should exist";

    cameraPolesFile.open(QIODeviceBase::ReadOnly);
    ASSERT_TRUE(cameraPolesFile.isOpen()) << "Rationale: should be able to open test data";
    QByteArray cameraPolesData = cameraPolesFile.readAll();
    auto jsonDocument = QJsonDocument::fromJson(std::move(cameraPolesData));

    // WHEN: Constructing a CameraPole list
    auto cameraPoles = cameraPolesFromJson(jsonDocument);

    // THEN: CameraPole list should not be empty
    EXPECT_FALSE(cameraPoles.empty());

    // AND: The first CameraPole should have the expected properties
    const auto& cameraPole = cameraPoles[0];
    EXPECT_EQ(cameraPole.name, QString("I-44 & I-240"));

    const auto& camera = cameraPole.cameras;
    EXPECT_EQ(camera.size(), size_t{2}) << "Rationale: Camera pole should have two traffic cams";
}
