// SPDX-FileCopyrightText: 2024 - 2027 UnionTech Software Technology Co., Ltd.
// SPDX-License-Identifier: GPL-3.0-or-later
import QtQuick 2.0
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.15
import QtQuick.Window 2.15
import org.deepin.dtk 1.0
import org.deepin.dtk.style 1.0 as DS

import org.deepin.dcc 1.0

DccObject {
    id: root
    function toPercent(value: string) {
        return Number(value * 100).toFixed(0) + "%"
    }
    DccTitleObject {
        name: "output"
        parentName: "sound/outPut"
        displayName: qsTr("Output")
        weight: 10
    }

    DccObject {
        name: "noOutput"
        parentName: "sound/outPut"
        weight: 20
        pageType: DccObject.Item
        backgroundType: DccObject.Normal
        visible: dccData.model().outPutPortCombo.length === 0
        page: Column {
            width: parent.width
            Label {
                height: 100
                width: parent.width
                Layout.leftMargin: 10
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
                font: DTK.fontManager.t4
                text: qsTr("No output device for sound found")
            }
        }
    }

    DccObject {
        name: "outputGroup"
        parentName: "sound/outPut"
        weight: 30
        visible: dccData.model().outPutPortCombo.length !== 0
        pageType: DccObject.Item
        page: DccGroupView {
            height: implicitHeight + 10
            spacing: 0
        }
        DccObject {
            name: "outputVolume"
            parentName: "sound/outPut/outputGroup"
            displayName: qsTr("Output Volume")
            weight: 10
            pageType: DccObject.Editor
            page: RowLayout {
                Label {
                    font: DTK.fontManager.t7
                    text: root.toPercent(voiceTipsSlider.value)
                }
                IconButton {
                    Layout.alignment: Qt.AlignVCenter
                    icon {
                        name: dccData.model().speakerOn ? "sound_off" : "small_volume"
                        width: 16
                        height: 16
                    }
                    implicitWidth: 24
                    flat: !hovered

                    onClicked: {
                        dccData.worker().setSinkMute()
                    }
                }
                Slider {
                    id: voiceTipsSlider
                    Layout.alignment: Qt.AlignVCenter
                    implicitHeight: 24
                    handleType: Slider.HandleType.NoArrowHorizontal
                    highlightedPassedGroove: true
                    value: dccData.model().speakerVolume
                    to: dccData.model().increaseVolume ? 1.5 : 1.0
                    onValueChanged: {
                        if (voiceTipsSlider.value != dccData.model().speakerVolume) {
                            dccData.worker().setSinkVolume(voiceTipsSlider.value)
                        }
                    }
                }
                IconButton {
                    icon {
                        name: "big_volume"
                        width: 16
                        height: 16
                    }
                    implicitWidth: 24
                    background: Rectangle {
                        color: "transparent" // 背景透明
                        border.color: "transparent" // 边框透明
                        border.width: 0
                    }
                }
            }
        }
        DccObject {
            name: "volumeEnhancement"
            parentName: "sound/outPut/outputGroup"
            displayName: qsTr("Volume Boost")
            description: qsTr("If the volume is louder than 100%, it may distort audio and be harmful to output devices")
            weight: 20
            pageType: DccObject.Editor
            page: Switch {
                Layout.alignment: Qt.AlignRight
                checked: dccData.model().increaseVolume

                onCheckedChanged: {
                    dccData.worker().setIncreaseVolume(checked)
                }
            }
        }
        DccObject {
            name: "volumeBalance"
            parentName: "sound/outPut/outputGroup"
            displayName: qsTr("Left Right Balance")
            weight: 30
            pageType: DccObject.Editor
            page: RowLayout {
                Label {
                    Layout.alignment: Qt.AlignVCenter
                    font: DTK.fontManager.t7
                    Layout.topMargin: 2
                    text: qsTr("Left")
                }
                Slider {
                    id: balanceSlider
                    Layout.alignment: Qt.AlignVCenter
                    implicitHeight: 24
                    from: -1
                    handleType: Slider.HandleType.ArrowBottom
                    highlightedPassedGroove: true
                    stepSize: 0.00001
                    to: 1
                    value: dccData.model().speakerBalance

                    onValueChanged: {
                        if (balanceSlider.value != dccData.model().speakerBalance) {
                            dccData.worker().setSinkBalance(balanceSlider.value)
                        }
                    }
                }
                Label {
                    Layout.alignment: Qt.AlignVCenter
                    Layout.topMargin: 2
                    font: DTK.fontManager.t7
                    text: qsTr("Right")
                }
            }
        }
        DccObject {
            name: "monoAudio"
            parentName: "sound/outPut/outputGroup"
            displayName: qsTr("Mono audio")
            description: qsTr("Merge left and right channels into a single channel")
            weight: 40
            pageType: DccObject.Editor
            page: Switch {
                checked: dccData.model().audioMono
                onCheckedChanged: {
                    dccData.worker().setAudioMono(checked)
                }
            }
        }
        DccObject {
            name: "plugAndUnplugManagement"
            parentName: "sound/outPut/outputGroup"
            displayName: qsTr("Auto pause")
            description: qsTr("Whether the audio will be automatically paused when the current audio device is unplugged")
            weight: 50
            pageType: DccObject.Editor
            page: Switch {
                checked: dccData.model().pausePlayer
                onCheckedChanged: {
                    dccData.worker().setPausePlayer(checked)
                }
            }
        }
        DccObject {
            name: "outputDevice"
            parentName: "sound/outPut/outputGroup"
            displayName: qsTr("Output Devices")
            weight: 60
            pageType: DccObject.Editor
            page: ComboBox {
                id: control
                Layout.alignment: Qt.AlignRight
                Layout.rightMargin: 10
                flat: true
                textRole: "name"

                currentIndex: dccData.model().outPutPortComboIndex
                model: dccData.model().soundOutputDeviceModel()
                enabled: dccData.model().outPutPortComboEnable

                property bool isInitialized: false

                delegate: MenuItem {
                    id: menuItem
                    useIndicatorPadding: true
                    width: control.width
                    text: model.name
                    visible: model.isEnabled
                    icon.name: (control.iconNameRole && model[control.iconNameRole] !== undefined) ? model[control.iconNameRole] : null
                    highlighted: control.highlightedIndex === index
                    hoverEnabled: control.hoverEnabled
                    autoExclusive: true
                    checked: control.currentIndex === index
                    implicitHeight: visible ? DS.Style.control.implicitHeight(menuItem) : 0

                    onClicked: {
                        dccData.worker().setActivePort(index, 1)
                    }
                }
            }
        }

        DccObject {
            name: "bluetoothMode"
            parentName: "sound/outPut/outputGroup"
            displayName: qsTr("Mode")
            weight: 60
            pageType: DccObject.Editor
            visible: dccData.model().showBluetoothMode
            page: ComboBox {
                id: bluetoothModeCombo
                Layout.alignment: Qt.AlignRight
                Layout.rightMargin: 10
                flat: true
                model: dccData.model().bluetoothModeOpts
                currentIndex: indexOfValue(dccData.model().currentBluetoothAudioMode)
                property bool isInitialized: false
                // 等待组件加载完成后，设置 isInitialized 为 true
                Component.onCompleted: {
                    console.log("bluetoothModeCombo onCompleted:", isInitialized)
                    incrementCurrentIndex()
                    isInitialized = true
                }
                onCurrentIndexChanged: {
                    console.log("bluetoothModeCombo Selected index:", currentIndex, isInitialized)
                    if (isInitialized) {
                        dccData.worker().setBluetoothMode(valueAt(currentIndex))
                    }
                }
            }
        }
    }
}
