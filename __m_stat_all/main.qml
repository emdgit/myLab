import QtQuick 2.9
import QtQuick.Window 2.2
import QtQuick.Controls 2.2

Window {
    visible: true
    width: 640
    height: 480
    title: qsTr("Hello World")

    Rectangle
    {
        id: canvas
        color: "black"
        anchors.fill: parent
        focus: true

        About
        {
            id: aboutDlg
        }
    }

    Row
    {
      anchors.right: parent.right;
      anchors.rightMargin: 4;
      anchors.top: parent.top;
      anchors.topMargin: 4;

      spacing: 4

      WindowButton
      {
        id: about;

        source: "about.png";

        function callback()
        {
            mObject.qmlAbout()
            aboutDlg.show()
        }
      }

      WindowButton
      {
        id: exit;

        source: "exit.png";

        function callback()
        {
            mObject.qmlQuit()
        }
      }
    }

}


