#include <QFile>
#include <QFileInfo>
#include <QJsonDocument>
#include <gtest/gtest.h>

#include <SpatialReference.h>

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
    ASSERT_FALSE(cameraPoles.empty());

    // AND: The first CameraPole should have the expected properties
    const auto& cameraPole = cameraPoles[0];
    EXPECT_EQ(cameraPole.name, QString("I-44 & I-240"));
    EXPECT_EQ(cameraPole.location, Esri::ArcGISRuntime::Point(-97.57406, 35.39637, Esri::ArcGISRuntime::SpatialReference::wgs84()));

    const auto& cameras = cameraPole.cameras;
    EXPECT_EQ(cameras.size(), size_t{2}) << "Rationale: Camera pole should have two traffic cams";

    auto camera_it = cameras.find(CardinalDirection::S);
    EXPECT_NE(camera_it, std::end(cameras)) << "Rationale: Camera pole should have a south-facing traffic cam";

    const auto& camera = camera_it->second;
    EXPECT_EQ(camera.name, "I-44 & I-240 S");
    EXPECT_EQ(camera.location, Esri::ArcGISRuntime::Point(-97.57406, 35.39637, Esri::ArcGISRuntime::SpatialReference::wgs84()));
}
