/*
 * Copyright (C) 2015 ~ 2017 Deepin Technology Co., Ltd.
 *
 * Author:     sbw <sbw@sbw.so>
 *             kirigaya <kirigaya@mkacg.com>
 *             Hualet <mr.asianwang@gmail.com>
 *
 * Maintainer: sbw <sbw@sbw.so>
 *             kirigaya <kirigaya@mkacg.com>
 *             Hualet <mr.asianwang@gmail.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

/*
 * This file was generated by qdbusxml2cpp version 0.8
 * Command line was: qdbusxml2cpp dde-center -a dbuscontrolcenter -c DBusControlCenter -l Frame
 *
 * qdbusxml2cpp is Copyright (C) 2015 Digia Plc and/or its subsidiary(-ies).
 *
 * This is an auto-generated file.
 * Do not edit! All changes made to it will be lost.
 */

#include "dbuscontrolcenterservice.h"
#include "frame.h"

#include <QtCore/QMetaObject>
#include <QtCore/QByteArray>
#include <QtCore/QList>
#include <QtCore/QMap>
#include <QtCore/QString>
#include <QtCore/QStringList>
#include <QtCore/QVariant>
#include <QGuiApplication>
#include <QScreen>
#include <QString>

/*
 * Implementation of adaptor class DBusControlCenter
 */

DBusControlCenterService::DBusControlCenterService(Frame *parent)
    : QDBusAbstractAdaptor(parent),
      m_toggleProcessed(true)
{
    // constructor

//    connect(parent, &Frame::xChanged, [parent]{
//        QDBusMessage m = QDBusMessage::createSignal("/com/deepin/dde/ControlCenter", "org.freedesktop.DBus.Properties", "PropertiesChanged");
//        QMap<QString, QVariant> changedProperties;
//        changedProperties["X"] = parent->visibleFrameXPos();
//        QList<QString> invalidatedProperties;
//        m << QVariant("com.deepin.dde.ControlCenter") << changedProperties << QVariant(invalidatedProperties);
//        QDBusConnection::sessionBus().send(m);
//    });

//    connect(parent, &Frame::hideInLeftChanged, parent, &Frame::xChanged);

    connect(parent, &Frame::rectChanged, this, &DBusControlCenterService::rectChanged);
}

DBusControlCenterService::~DBusControlCenterService()
{
    // destructor
}

Frame *DBusControlCenterService::parent() const
{
    return static_cast<Frame *>(QObject::parent());
}

bool DBusControlCenterService::showInRight() const
{
    // get the value of property ShowInRight
    // FIXME
//    return qvariant_cast< bool >(!parent()->isHideInLeft());
    return true;
}

const QRect DBusControlCenterService::rect() const
{
    return parent()->geometry();
}

//int DBusControlCenterService::x() const
//{
//    if (!parent()->isVisible())
//        return qApp->primaryScreen()->geometry().right();

//    return parent()->x();
//}

void DBusControlCenterService::Hide()
{
    // handle method call com.deepin.dde.ControlCenter.Hide
    parent()->hide();
}

void DBusControlCenterService::HideImmediately()
{
    // handle method call com.deepin.dde.ControlCenter.HideImmediately
    parent()->hideImmediately();
}

void DBusControlCenterService::Show()
{
    // handle method call com.deepin.dde.ControlCenter.Show
#ifdef DISABLE_MAIN_PAGE
    parent()->showSettingsPage(QString(), QString());
#else
    parent()->show();
#endif
}

void DBusControlCenterService::ShowImmediately()
{
    // handle method call com.deepin.dde.ControlCenter.ShowImmediately
#ifdef DISABLE_MAIN_PAGE
    parent()->showSettingsPage(QString(), QString());
#else
    parent()->show();
#endif
}

void DBusControlCenterService::ShowHome()
{
    parent()->backToHome();
}

void DBusControlCenterService::ShowModule(const QString &module)
{
    ShowPage(module, QString());
}

void DBusControlCenterService::ShowPage(const QString &module, const QString &page)
{
    // handle method call com.deepin.dde.ControlCenter.ShowModule
    parent()->showSettingsPage(module, page);
}

void DBusControlCenterService::SetAutoHide(const bool autoHide)
{
    parent()->setDebugAutoHide(autoHide);
}

void DBusControlCenterService::Toggle()
{
    // handle method call com.deepin.dde.ControlCenter.Toggle

    // drop incoming toggle requests until the previous request
    // has been processed.
    if (m_toggleProcessed) {

        QTimer::singleShot(0, this, [this] {
            parent()->toggle();
            m_toggleProcessed = true;
        });

        m_toggleProcessed = false;
    }
}

void DBusControlCenterService::ToggleInLeft()
{
    // handle method call com.deepin.dde.ControlCenter.ToggleInLeft
    Toggle();
}

bool DBusControlCenterService::isNetworkCanShowPassword()
{
    // handle method call com.deepin.dde.ControlCenter.isNetworkCanShowPassword
    //return parent()->isNetworkCanShowPassword();

    return false; // for remove gcc warning.
}

