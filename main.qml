import QtQuick 2.4
import QtQuick.Controls 1.3
import QtQuick.Window 2.2
import QtQuick.Dialogs 1.2
import QtQuick.Layouts 1.1
import QtCharts 2.0

// Ez lesz az alkalmazás főablaka
ApplicationWindow {
    title: qsTr("Simulator Controller")
    width: 1400
    height: 720
    visible: true

    menuBar: MenuBar {
        Menu {
            title: qsTr("&Exit")
            MenuItem {
                text: qsTr("E&xit")
                onTriggered: Qt.quit();
            }
        }
    }

    signal positionChanged(real x, real y)
    onPositionChanged:{
        positionData.append(x,y)
    }

    signal distanceChanged(real distance)
    onDistanceChanged:{
        if (distanceData.count == 20)
        {
            distanceData.removePoints(0,1);
            axisXd.max = axisXd.max+1
            axisXd.min = axisXd.min+1
        }
        var x = distanceData.count < 19 ? distanceData.count : axisXd.max
        distanceData.append(x,distance)
    }

    signal colourChanged(real c1t,real c2t,real c3t, real c4t,real c5t,real c6t)
    onColourChanged:{
        c1BarSet.remove(0, 1)
        c1BarSet.append(c1t)
        c2BarSet.remove(0, 1)
        c2BarSet.append(c2t)
        c3BarSet.remove(0, 1)
        c3BarSet.append(c3t)
        c4BarSet.remove(0, 1)
        c4BarSet.append(c4t)
        c5BarSet.remove(0, 1)
        c5BarSet.append(c5t)
        c6BarSet.remove(0, 1)
        c6BarSet.append(c6t)
    }

 // Remote car control
    GroupBox {
        id: commandsGB
        title: "Car control"
        anchors.left : parent.left
        anchors.top : parent.top
        width: 200

        // Buttons
        ColumnLayout {
            id: columnLayoutCommands

            anchors.fill: parent

            Button {
                id: selfTestBtn
                objectName: "selfTestButton"
                anchors.left: parent.left
                anchors.right: parent.right
                text: qsTr("Self Test")
                anchors.leftMargin: 0
                anchors.rightMargin: 0
            }
            Button {
                id: resetBtn
                objectName: "resetButton"
                anchors.left: parent.left
                anchors.right: parent.right
                text: qsTr("Reset")
                anchors.leftMargin: 0
                anchors.rightMargin: 0
            }
            Button {
                id: autoBtn
                objectName: "autoButton"
                anchors.left: parent.left
                anchors.right: parent.right
                text: qsTr("Auto control")
                anchors.rightMargin: 0
                anchors.leftMargin: 0
            }
            Text{
                text: qsTr("Velocity")
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.rightMargin: 0
                anchors.leftMargin: 0
            }
            Slider {
                id: velocitySlider
                objectName: "velocitySlider"
                tickmarksEnabled: true
                minimumValue: -1
                maximumValue: 1
                stepSize: 1
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.rightMargin: 0
                anchors.leftMargin: 0
            }
            Text{
                text: qsTr("Steering")
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.rightMargin: 0
                anchors.leftMargin: 0
            }
            Slider {
                id: steeringSlider
                objectName: "steeringSlider"
                tickmarksEnabled: true
                minimumValue: -1
                maximumValue: 1
                stepSize: 1
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.rightMargin: 0
                anchors.leftMargin: 0
            }
        }
    }

    // TCP communication
     GroupBox {
         id: communicationGB
         title: "TCP communication"
         anchors.left : parent.left
         anchors.top: commandsGB.bottom
         width: 200

         ColumnLayout {
             id: columnLayoutTCP

             anchors.fill: parent

            Text{
                text: qsTr("Incoming")
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.rightMargin: 0
                anchors.leftMargin: 0
            }
            TextArea{
                objectName: "incomingMessage"
                readOnly: true
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.rightMargin: 0
                anchors.leftMargin: 0
                Layout.preferredHeight: 40
            }
            Text{
                text: qsTr("Outgoing")
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.rightMargin: 0
                anchors.leftMargin: 0
            }
            TextArea{
                objectName: "outgoingMessage"
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.rightMargin: 0
                anchors.leftMargin: 0
                Layout.preferredHeight: 40
            }
            Button{
                objectName: "sendButton"
                text: qsTr("Send")
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.rightMargin: 0
                anchors.leftMargin: 0
            }
         }
     }

     // Delegate: this is the appearance of a list item
     // A későbbi history listának szüksége van egy delegate-re. Minden lista elem ennek a komponensnek egy
     //  példánya lesz.
     Component {
         // ID, hogy tudjuk a listánál hivatkozni erre, mint a lista delegatejére.
         id: stateDelegate
         Row {
             // Itt a model az, ami a list egyik eleme. (Bármi is legyen majd az.)
             Text { text: " Speed: " + model.sp.toFixed(3) }
             Text { text: " Steering: " + model.st.toFixed(3) }
         }
     }
     // Position (text)
     GroupBox {
         id: positionGB
         title: "Movement"
         width: 200
         anchors.left : parent.left
         anchors.top : communicationGB.bottom
         anchors.bottom: parent.bottom

         ScrollView {
             // A scrollohzató elem igazítása a szölő RowLayouthoz.
             // Itt a ScrollViewon belül adjuk meg, hogy a RowLayoutban
             //  mik legyenek a rá (ScrollViewra) vonatkozó méret beállítások,
             //  mert ezeket a RowLayout kezeli ebben az esetben.
             anchors.top: parent.top
             anchors.bottom: parent.bottom

             // Itt jön a tényleges lista.
             ListView {
                 id: stateHistoryList
                 // A model az, ahonnan az adatokat vesszük.
                 // A historyModel változót a MainWindowsEventHandling::historyChanged metódus teszi
                 //  elérhetővé a QML oldalon is.
                 //  Típusa QList<QObject*>, a tárolt pointerek valójában RobotState-ekre mutatnak.
                 model: historyModel
                 // A delegate megadása, vagyis hogy egy listaelem hogyan jelenjen meg.
                 //  (Már fentebb definiáltuk.)
                 delegate: stateDelegate

                 // Eseménykezelő, az elemek darabszámának változása esetén a kijelölést
                 //  a legalsó elemre viszi. Ezzel oldja meg, hogy folyamatosan scrollozódjon
                 //  a lista és a legutoló elem mindig látható legyen.
                 onCountChanged: {
                     stateHistoryList.currentIndex = stateHistoryList.count - 1;
                 }
             }
         }
     }

      // Diagnostics
      GroupBox {
          id: diagnosticsGB
          title: "Diagnostics"
          anchors.right: parent.right
          anchors.rightMargin: 0
          anchors.left : commandsGB.right
          anchors.top : parent.top
          anchors.bottom: parent.bottom

          Button{
              objectName: "startButton"
              onClicked:  text = (text === qsTr("Stop diagnosis") ? qsTr("Start diagnosis") : qsTr("Stop diagnosis"))
              text: qsTr("Start diagnosis")
              anchors.left: parent.left
              anchors.right: parent.right
              anchors.rightMargin: 0
              anchors.leftMargin: 0
          }

          RowLayout
          {
              anchors.right: parent.right
              anchors.rightMargin: 0
              anchors.topMargin: 30
              anchors.left : parent.left
              anchors.top : parent.top
              anchors.bottom: parent.bottom

             ColumnLayout
             {
                anchors.rightMargin: 0
                 anchors.left : parent.left
                 anchors.top : parent.top
                 anchors.bottom: parent.bottom

                 ChartView {
                     id: distancePlot
                     width: 500
                     Layout.fillWidth: true
                     Layout.fillHeight: true
                     title: "Distance"
                     antialiasing: true

                     ValueAxis {
                            id: axisXd
                            min: 0
                            max: 19
                        }

                     ValueAxis {
                            id: axisYd
                            min: 0
                            max: 50
                        }

                     LineSeries {
                         id: distanceData
                         name: "distance"
                         axisX: axisXd
                         axisY: axisYd
                     }
                 }

                 ChartView {
                     id: colourPlot
                     width: 500
                     Layout.fillWidth: true
                     Layout.fillHeight: true
                     antialiasing: true

                     BarSeries {
                            id: mySeries
                            axisX: BarCategoryAxis { categories: ["Light sensors"] }
                            axisY: ValueAxis { id:axisY; min: 0; max: 1; tickCount: 5 }
                            BarSet { label: "C1"; values: [0]; id: c1BarSet }
                            BarSet { label: "C2"; values: [0]; id: c2BarSet }
                            BarSet { label: "C3"; values: [0]; id: c3BarSet }
                            BarSet { label: "C4"; values: [0]; id: c4BarSet }
                            BarSet { label: "C5"; values: [0]; id: c5BarSet }
                            BarSet { label: "C6"; values: [0]; id: c6BarSet }
                        }
                   }
              }

             ColumnLayout
             {
                 anchors.rightMargin: 0
                 anchors.left: distancePlot.right
                 anchors.right : parent.right
                 anchors.top : parent.top
                 anchors.bottom: parent.bottom

                 ChartView {
                     id: positionPlot
                     width: 670
                     Layout.fillWidth: true
                     Layout.fillHeight: true
                     title: "Position"
                     antialiasing: true

                     ValueAxis {
                            id: axisPosXd
                            min: 0
                            max: 50
                        }

                     ValueAxis {
                            id: axisPosYd
                            min: 0
                            max: 50
                        }

                     LineSeries {
                         id: positionData
                         name: "position"
                         axisX: axisPosXd
                         axisY: axisPosYd
                     }
                 }
              }
          }
     }
}
