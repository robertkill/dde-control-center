// SPDX-FileCopyrightText: 2024 - 2027 UnionTech Software Technology Co., Ltd.
// SPDX-License-Identifier: GPL-3.0-or-later
import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

import org.deepin.dtk 1.0 as D

DccEditorItem {
    id: control
    property var editor: null
    leftPadding: 12
    rightPadding: 10
    topPadding: topInset
    bottomPadding: bottomInset
    implicitHeight: 58
    rightItem: RowLayout {
        Control {
            contentItem: editor ? editor : model.item.getSectionItem(this)
        }

        D.IconLabel {
            icon.name: "arrow_ordinary_right"
            icon.palette: D.DTK.makeIconPalette(control.palette)
            icon.mode: control.D.ColorSelector.controlState
            icon.theme: control.D.ColorSelector.controlTheme
        }
    }
    onClicked: {
        if (model.item.children.length > 0) {
            DccApp.showPage(model.item.children[0])
        } else {
            console.warn(model.item.name, " MenuEditor nust include children", model.item.children.length)
        }
    }
}
