/*
 * Copyright (C) 2011 ~ 2017 Deepin Technology Co., Ltd.
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

#ifndef DISPLAYMODEL_H
#define DISPLAYMODEL_H

#include <QObject>

#include "monitor.h"

namespace dcc {

namespace display {

class DisplayWorker;
class DisplayModel : public QObject
{
    Q_OBJECT

    friend class DisplayWorker;

public:
    explicit DisplayModel(QObject *parent = 0);

    int screenHeight() const { return m_screenHeight; }
    int screenWidth() const { return m_screenWidth; }
    int displayMode() const { return m_mode; }
    double uiScale() const { return m_uiScale; }
    const QString primary() const { Q_ASSERT(!m_primary.isEmpty()); return m_primary; }
    const QString config() const { return m_currentConfig; }
    const QStringList configList() const { return m_configList; }
    const QList<Monitor *> monitorList() const { return m_monitors; }
    const QList<Resolution> monitorsSameModeList() const;
    Monitor *primaryMonitor() const;

    bool monitorsIsIntersect() const;

    bool isNightMode() const;
    void setIsNightMode(bool isNightMode);

    bool redshiftIsValid() const;

signals:
    void screenHeightChanged(const int h) const;
    void screenWidthChanged(const int w) const;
    void displayModeChanged(const int mode) const;
    void uiScaleChanged(const double scale) const;
    void primaryScreenChanged(const QString &primary) const;
    void currentConfigChanged(const QString &config) const;
    void firstConfigCreated(const QString &config) const;
    void configListChanged(const QStringList &configs) const;
    void monitorListChanged() const;
    void nightModeChanged(const bool nightmode) const;
    void redshiftVaildChanged(const bool isvalid) const;

private slots:
    void setScreenHeight(const int h);
    void setScreenWidth(const int w);
    void setDisplayMode(const int mode);
    void setUIScale(const double scale);
    void setPrimary(const QString &primary);
    void setCurrentConfig(const QString &config);
    void setConfigList(const QStringList &configList);
    void setRedshiftIsValid(bool redshiftIsValid);
    void monitorAdded(Monitor *mon);
    void monitorRemoved(Monitor *mon);

private:
    int m_screenHeight;
    int m_screenWidth;
    int m_mode;
    double m_uiScale;
    QString m_primary;
    QString m_currentConfig;
    QStringList m_configList;
    QList<Monitor *> m_monitors;
    bool m_isNightMode;
    bool m_redshiftIsValid;
};

} // namespace display

} // namespace dcc

#endif // DISPLAYMODEL_H
