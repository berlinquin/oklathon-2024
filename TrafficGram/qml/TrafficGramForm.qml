// Copyright 2024 ESRI
//
// All rights reserved under the copyright laws of the United States
// and applicable international laws, treaties, and conventions.
//
// You may freely redistribute and use this sample code, with or
// without modification, provided you include the original copyright
// notice and use restrictions.
//
// See the Sample code usage restrictions document for further information.
//

import QtQuick
import QtQuick.Controls
import QtMultimedia
import Esri.TrafficGram

Item {

    // Create MapQuickView here, and create its Map etc. in C++ code
    MapView {
        id: view
        anchors.fill: parent
        // set focus to enable keyboard navigation
        focus: true
    }

    // Declare the C++ instance which creates the map etc. and supply the view
    TrafficGram {
        id: model
        mapView: view
    }

    Rectangle {
        id: videoRectangle
        width: 200
        height: 150
        color:"black"
        anchors.bottom: parent.bottom
        anchors.left: parent.left

        visible: false

        MediaPlayer {
            id: mediaPlayer
            source:""
            videoOutput: videoOutput
            autoPlay: true
        }

        VideoOutput {
            id:videoOutput
            anchors.fill: parent
        }

        Connections {
            target: model
            onShowVideo: {
                videoRectangle.visible = true;
                mediaPlayer.source = urlString;
            }
            onHideVideo: {
                videoRectangle.visible = false;
            }
        }
    }

    Rectangle {
        id: textObject
        anchors.bottom: videoRectangle.top
        color:"white"
        width: 200
        height: 15
        Text {
            id: actualText
            text: "Hello world camera"
            anchors.fill:parent
        }

        visible: false

        Connections {
            target: model
            onShowVideo: {
                textObject.visible = true
                actualText.text = stationName
            }
            onHideVideo: {
                textObject.visible = false
            }
        }
    }

    Button {
        id: startSimulatedLocation
        text: "Start Simulated Location"
        anchors.bottom: parent.bottom
        anchors.right: parent.right
        anchors.margins: 5
        onClicked: model.startSimulatedLocation()

        background: Rectangle {
            border.color: "black"                                 // Set the border color
            color: parent.pressed ? "light gray" : "black"        // Background color of the button
            radius: 5                                             // Optional: rounded corners
        }
    }
}
