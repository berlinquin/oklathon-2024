# TrafficGram

A project for the Oklathon hackathon on October 19, 2024.

## Project goal

- The Oklahoma Dept of Transportation (ODOT) provides live camera streams at
many locations along the highway.
- Our plan is to make a mobile app that can run in the background when the user
is driving, recognize when the user's location is near a traffic cam, and record
a few seconds of footage from the traffic cam, which is saved to the user's
phone.
- This recording can be saved in case of a traffic accident, be shared on social
  media, or be used to produce a time-lapse of a daily commute.

 Our hope is that the TrafficGram app would allow for unique media projects and maybe add some fun to a
daily commute!

## Demo

![App demo](./resources/demo.gif)

The gif aboves shows the progress of our app by the end of the hackathon. You can observe:

- The blue dot, simulating the user's location, moving along the highway (I-44).
- As it nears a traffic cam (represented by a red dot) an inlay pops up showing a live feed from the traffic cam at that location.

## Dev Environment

Follow these steps to set up your dev environment. This assumes development is being done on macOS.

1. The app will rely on the [ArcGIS Maps SDK for Qt](https://developers.arcgis.com/qt/). To set up the SDK, follow their [Install and set up](https://developers.arcgis.com/qt/install-and-set-up/#install-on-macos) instructions. This has you install the following components:
   - Apple Xcode (16.0)
     - I also installed the iOS component (iOS 18.0 Simulator 22A3351)
   - The Qt Framework for macOS (6.8.0)
   - The Qt Framework for iOS
     - (I don't think this is a separate install)
   - Qt Creator (14.0.2)
   - CMake (3.29.3)
     - Installed with Qt Creator. Should not need to install separately.
   - ArcGIS Maps SDK for Qt for macOS (200.5.0)

2. Do the [Display a map](https://developers.arcgis.com/qt/maps-2d/tutorials/display-a-map/) tutorial to verify everything is set up.
   - Optionally, create your own [Location Platform](https://www.esri.com/en-us/arcgis/products/arcgis-location-platform/overview) account. I have an account and API key that we can use for the hackathon.
   - I used cmake as the build tool and it worked fine.

## Links

- The [oktraffic.org](https://oktraffic.org/#/map) map with the different traffic cams
