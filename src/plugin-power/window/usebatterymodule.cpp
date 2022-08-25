/*
* Copyright (C) 2021 ~ 2021 Deepin Technology Co., Ltd.
*
* Author:     caixiangrong <caixiangrong@uniontech.com>
*
* Maintainer: caixiangrong <caixiangrong@uniontech.com>
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
#include "usebatterymodule.h"
#include "dcombobox.h"
#include "dswitchbutton.h"
#include "itemmodule.h"
#include "powermodel.h"
#include "powerworker.h"
#include "settingsgroupmodule.h"
#include "src/plugin-notification/window/notificationwidget.h"
#include "titlemodule.h"
#include "titlevalueitem.h"
#include "widgets/comboxwidget.h"
#include "widgets/titledslideritem.h"
#include "widgets/dccslider.h"

#include <DConfig>

#include <QLayout>

DCC_USE_NAMESPACE
DWIDGET_USE_NAMESPACE
DCORE_USE_NAMESPACE

const static QMap<int, int> g_sldLowBatteryMap = { { 0, 10 }, { 1, 15 }, { 2, 20 }, { 3, 25 } };

UseBatteryModule::UseBatteryModule(PowerModel *model, PowerWorker *work, QObject *parent)
    : PageModule("onBattery", tr("UseBattery"), QIcon::fromTheme("dcc_battery"), parent)
    , m_model(model)
    , m_work(work)
    , m_annos({ "1m", "5m", "10m", "15m", "30m", "1h", tr("Never") })
{
    connect(this, &UseBatteryModule::requestSetScreenBlackDelayOnBattery, m_work, &PowerWorker::setScreenBlackDelayOnBattery);
    connect(this, &UseBatteryModule::requestSetSleepDelayOnBattery, m_work, &PowerWorker::setSleepDelayOnBattery);
    connect(this, &UseBatteryModule::requestSetAutoLockScreenOnBattery, m_work, &PowerWorker::setLockScreenDelayOnBattery);

    connect(this, &UseBatteryModule::requestSetBatteryPressPowerBtnAction, m_work, &PowerWorker::setBatteryPressPowerBtnAction);
    connect(this, &UseBatteryModule::requestSetBatteryLidClosedAction, m_work, &PowerWorker::setBatteryLidClosedAction);
    connect(this, &UseBatteryModule::requestSetLowPowerNotifyEnable, m_work, &PowerWorker::setLowPowerNotifyEnable);
    connect(this, &UseBatteryModule::requestSetLowPowerNotifyThreshold, m_work, &PowerWorker::setLowPowerNotifyThreshold);
    connect(this, &UseBatteryModule::requestSetLowPowerAutoSleepThreshold, m_work, &PowerWorker::setLowPowerAutoSleepThreshold);

    initUI();
}

UseBatteryModule::~UseBatteryModule()
{
}

void UseBatteryModule::active()
{
    m_Options.clear();
    /*** 笔记本合盖功能与按电源按钮功能 ***/
    if (m_model->getShutdown()) {
        m_Options.append({ tr("Shut down"), 0 });
    }
    if (m_work->getCurCanSuspend()) {
        m_Options.append({ tr("Suspend"), 1 });
    }
    if (m_work->getCurCanHibernate()) {
        m_Options.append({ tr("Hibernate"), 2 });
    }
    m_Options.append({ tr("Turn off the monitor"), 3 });
    m_Options.append({ tr("Do nothing"), 4 });
}

void UseBatteryModule::deactive()
{
}

void UseBatteryModule::initUI()
{
    auto delayToLiteralString = [] (const int delay) -> QString const {
        QString strData = "";

        switch (delay) {
        case 1:
            strData = tr("1 Minute");
            break;
        case 2:
            strData = tr("%1 Minutes").arg(5);
            break;
        case 3:
            strData = tr("%1 Minutes").arg(10);
            break;
        case 4:
            strData = tr("%1 Minutes").arg(15);
            break;
        case 5:
            strData = tr("%1 Minutes").arg(30);
            break;
        case 6:
            strData = tr("1 Hour");
            break;
        case 7:
            strData = tr("Never");
            break;
        default:
            strData = tr("%1 Minutes").arg(15);
            break;
        }

        return strData;
    };

    appendChild(new TitleModule("screenLockAndSleep", tr("Screen lock and sleep")));
    SettingsGroupModule *group = new SettingsGroupModule("screenLockAndSleepGroup", tr("Screen lock and sleep"));
    group->setSpacing(10);
    appendChild(group);
    group->appendChild(new ItemModule("monitorSleepOnBattery", tr("Monitor will suspend after"),
        [this, delayToLiteralString] (ModuleObject *module) -> QWidget*{
            TitledSliderItem *monitorSleepOnBattery = new TitledSliderItem(tr("Monitor will suspend after"));
            monitorSleepOnBattery->setTitle(tr("Monitor will suspend after"));
            monitorSleepOnBattery->setAccessibleName(tr("Monitor will suspend after"));
            monitorSleepOnBattery->slider()->setType(DCCSlider::Vernier);
            monitorSleepOnBattery->slider()->setRange(1, 7);
            monitorSleepOnBattery->slider()->setTickPosition(QSlider::TicksBelow);
            monitorSleepOnBattery->slider()->setTickInterval(1);
            monitorSleepOnBattery->slider()->setPageStep(1);
            monitorSleepOnBattery->setAnnotations(m_annos);
            monitorSleepOnBattery->addBackground();
            auto setScreenBlackDelayOnBattery = [monitorSleepOnBattery, &delayToLiteralString] (const int delay) {
                monitorSleepOnBattery->slider()->blockSignals(true);
                monitorSleepOnBattery->slider()->setValue(delay);
                monitorSleepOnBattery->setValueLiteral(delayToLiteralString(delay));
                monitorSleepOnBattery->slider()->blockSignals(false);
            };
            connect(m_model, &PowerModel::screenBlackDelayChangedOnBattery, monitorSleepOnBattery, setScreenBlackDelayOnBattery);
            setScreenBlackDelayOnBattery(m_model->screenBlackDelayOnBattery());
            connect(monitorSleepOnBattery->slider(), &DCCSlider::valueChanged, this, &UseBatteryModule::requestSetScreenBlackDelayOnBattery);
            return monitorSleepOnBattery;
        }, false));

    group->appendChild(new ItemModule("monitorSleepOnPower", tr("Lock screen after"),
        [this, delayToLiteralString] (ModuleObject *module) -> QWidget*{
            TitledSliderItem *autoLockScreen = new TitledSliderItem(tr("Lock screen after"));
            autoLockScreen->setTitle(tr("Lock screen after"));
            autoLockScreen->setAccessibleName(tr("Lock screen after"));
            autoLockScreen->slider()->setType(DCCSlider::Vernier);
            autoLockScreen->slider()->setRange(1, 7);
            autoLockScreen->slider()->setTickPosition(QSlider::TicksBelow);
            autoLockScreen->slider()->setTickInterval(1);
            autoLockScreen->slider()->setPageStep(1);
            autoLockScreen->setAnnotations(m_annos);
            autoLockScreen->addBackground();
            auto setAutoLockScreenOnBattery = [autoLockScreen, &delayToLiteralString] (const int delay) {
                autoLockScreen->slider()->blockSignals(true);
                autoLockScreen->slider()->setValue(delay);
                autoLockScreen->setValueLiteral(delayToLiteralString(delay));
                autoLockScreen->slider()->blockSignals(false);
            };
            connect(m_model, &PowerModel::batteryLockScreenDelayChanged, autoLockScreen, setAutoLockScreenOnBattery);
            setAutoLockScreenOnBattery(m_model->getBatteryLockScreenDelay());
            connect(autoLockScreen->slider(), &DCCSlider::valueChanged, this, &UseBatteryModule::requestSetAutoLockScreenOnBattery);
            return autoLockScreen;
        }, false));

    group->appendChild(new ItemModule("computerSleepOnBattery", tr("Computer will suspend after"),
        [this, delayToLiteralString] (ModuleObject *module) -> QWidget*{
            TitledSliderItem *computerSleepOnBattery = new TitledSliderItem(tr("Computer will suspend after"));
            computerSleepOnBattery->setTitle(tr("Computer will suspend after"));
            computerSleepOnBattery->setAccessibleName(tr("Computer will suspend after"));
            computerSleepOnBattery->slider()->setType(DCCSlider::Vernier);
            computerSleepOnBattery->slider()->setRange(1, 7);
            computerSleepOnBattery->slider()->setTickPosition(QSlider::TicksBelow);
            computerSleepOnBattery->slider()->setTickInterval(1);
            computerSleepOnBattery->slider()->setPageStep(1);
            computerSleepOnBattery->setAnnotations(m_annos);
            computerSleepOnBattery->addBackground();
            auto setScreenBlackDelayOnBattery = [computerSleepOnBattery, &delayToLiteralString] (const int delay) {
                computerSleepOnBattery->slider()->blockSignals(true);
                computerSleepOnBattery->slider()->setValue(delay);
                computerSleepOnBattery->setValueLiteral(delayToLiteralString(delay));
                computerSleepOnBattery->slider()->blockSignals(false);
            };
            setScreenBlackDelayOnBattery(m_model->sleepDelayOnBattery());
            connect(m_model, &PowerModel::sleepDelayChangedOnBattery, computerSleepOnBattery, setScreenBlackDelayOnBattery);
            computerSleepOnBattery->setVisible(m_model->canSuspend() && m_model->getSuspend());
            connect(computerSleepOnBattery->slider(), &DCCSlider::valueChanged, this, &UseBatteryModule::requestSetSleepDelayOnBattery);
            return computerSleepOnBattery;
        }, false));

    group->appendChild(new ItemModule("cmbCloseLid", tr("When the lid is closed"),
        [this] (ModuleObject *module) -> QWidget*{
            AlertComboBox *cmbCloseLid = new AlertComboBox();
            auto setCloseLidData = [this, cmbCloseLid] () {
                updateComboxActionList();
                cmbCloseLid->blockSignals(true);
                cmbCloseLid->clear();
                for (const auto &it : qAsConst(m_Options)) {
                    if (it == m_Options.first())
                        continue;
                    cmbCloseLid->addItem(it.first, it.second);
                }
                for (int i = 0; i < cmbCloseLid->count(); i++) {
                    if (cmbCloseLid->itemData(i).toInt() == m_model->batteryLidClosedAction()) {
                        cmbCloseLid->setCurrentIndex(i);
                        break;
                    }
                }
                cmbCloseLid->blockSignals(false);
            };

            module->setHiden(!m_model->lidPresent());
            setCloseLidData();
            connect(cmbCloseLid, QOverload<int>::of(&AlertComboBox::currentIndexChanged), this, [this, cmbCloseLid](int index) {
                Q_EMIT requestSetBatteryLidClosedAction(cmbCloseLid->itemData(index).toInt());
            });
            connect(m_model, &PowerModel::batteryLidClosedActionChanged, cmbCloseLid, setCloseLidData);
            return cmbCloseLid;
        }));

    group->appendChild(new ItemModule("cmbPowerBtn", tr("When pressing the power button"),
        [this] (ModuleObject *module) -> QWidget*{
            AlertComboBox *cmbPowerBtn = new AlertComboBox();
            auto setPowerButtonData = [this, cmbPowerBtn] () {
                updateComboxActionList();
                cmbPowerBtn->blockSignals(true);
                cmbPowerBtn->clear();
                for (const auto &it : qAsConst(m_Options)) {
                    cmbPowerBtn->addItem(it.first, it.second);
                }
                for (int i = 0; i < cmbPowerBtn->count(); i++) {
                    if (cmbPowerBtn->itemData(i).toInt() == m_model->batteryPressPowerBtnAction()) {
                        cmbPowerBtn->setCurrentIndex(i);
                        break;
                    }
                }
                cmbPowerBtn->blockSignals(false);
            };
            setPowerButtonData();
            connect(cmbPowerBtn, QOverload<int>::of(&AlertComboBox::currentIndexChanged), this, [this, cmbPowerBtn](int index) {
                Q_EMIT requestSetBatteryPressPowerBtnAction(cmbPowerBtn->itemData(index).toInt());
            });
            connect(m_model, &PowerModel::batteryPressPowerBtnActionChanged, cmbPowerBtn, setPowerButtonData);
            return cmbPowerBtn;
        }));

    //　低电量设置
    appendChild(new TitleModule("lowBatteryMng", tr("Low Battery Management")));
    group = new SettingsGroupModule("lowBatteryMngGroup", tr("Low Battery Management"));
    group->setSpacing(10);
    appendChild(group);
    group->appendChild(new ItemModule("batteryHint", tr("Low Battery Notification"),
        [this] (ModuleObject *module) -> QWidget*{
            DSwitchButton *swBatteryHint = new DSwitchButton();
            swBatteryHint->setChecked(m_model->lowPowerNotifyEnable());
            connect(m_model, &PowerModel::lowPowerNotifyEnableChanged, swBatteryHint, &DSwitchButton::setChecked);

            connect(swBatteryHint, &DSwitchButton::checkedChanged, this, [=](bool bLowPowerNotifyEnable) {
                Q_EMIT requestSetLowPowerNotifyEnable(bLowPowerNotifyEnable);
            });
//            Q_EMIT swBatteryHint->checkedChanged(m_model->lowPowerNotifyEnable());
            return swBatteryHint;
        }));

    group->appendChild(new ItemModule("lowBatteryHint", tr("Low battery level"),
    [this] (ModuleObject *module) -> QWidget*{
        DComboBox *cmbLowBatteryHint = new DComboBox();
        cmbLowBatteryHint->setAccessibleName("cmbLowBatteryHint");
        module->setHiden(!m_model->haveBettary());
        QStringList levels;
        levels << "10%"
               << "15%"
               << "20%"
               << "25%";
        cmbLowBatteryHint->addItems(levels);
        if (m_model->lowPowerNotifyThreshold() < cmbLowBatteryHint->count()) {
            cmbLowBatteryHint->setCurrentIndex(m_model->lowPowerNotifyThreshold());
        }
        connect(cmbLowBatteryHint, QOverload<int>::of(&DComboBox::currentIndexChanged), this, [=](int value) {
            if (value < cmbLowBatteryHint->count())
                Q_EMIT requestSetLowPowerNotifyThreshold(value);
        });
        connect(m_model, &PowerModel::lowPowerNotifyEnableChanged, module, [module] (const bool state) {
            module->setHiden(!state);
        });
        connect(m_model, &PowerModel::lowPowerNotifyThresholdChanged, cmbLowBatteryHint, &DComboBox::setCurrentIndex);
        return cmbLowBatteryHint;
    }));

    group->appendChild(new ItemModule("autoSuspend", tr("Auto suspend battery level"),
        [this] (ModuleObject *module) -> QWidget*{
            DComboBox *cmbAutoSuspend = new DComboBox();
            cmbAutoSuspend->setAccessibleName("cmbAutoSuspend");
            module->setHiden(!m_model->getSuspend());
            QStringList levels;
            for (int i = 0; i < 9; i++) {
                levels.append(QString("%1\%").arg(i + 1));
            }
            cmbAutoSuspend->addItems(levels);
            if (m_model->lowPowerAutoSleepThreshold() < cmbAutoSuspend->count()) {
                cmbAutoSuspend->setCurrentIndex(m_model->lowPowerAutoSleepThreshold());
            }
            connect(cmbAutoSuspend, QOverload<int>::of(&DComboBox::currentIndexChanged), this, [=](int value) {
                if (value < cmbAutoSuspend->count())
                    Q_EMIT requestSetLowPowerAutoSleepThreshold(value);
            });
            connect(m_model, &PowerModel::lowPowerAutoSleepThresholdChanged, cmbAutoSuspend, [=] (const int value){
                cmbAutoSuspend->setCurrentIndex(value);
            });
            return cmbAutoSuspend;
        }));

    //　电池管理
    appendChild(new TitleModule("batterySettings", tr("batterySettings")));
    group = new SettingsGroupModule("batterySettingsGroup", tr("batterySettings"));
    group->setSpacing(10);
    appendChild(group);
    group->appendChild(new ItemModule("powerShowTimeToFull", tr("Display remaining using and charging time"),
        [] (ModuleObject *module) -> QWidget*{
            DSwitchButton *powerShowTimeToFull = new DSwitchButton();
            // depend dock dconfig setting "showtimetofull"
            DConfig *cfgDock = DConfig::create("org.deepin.dde.dock", "org.deepin.dde.dock", QString(), powerShowTimeToFull);
            connect(powerShowTimeToFull, &DSwitchButton::checkedChanged, powerShowTimeToFull, [cfgDock, powerShowTimeToFull] (){
                // 保存设置值
                if (!cfgDock->value("showtimetofull").isNull()) {
                    cfgDock->setValue("showtimetofull", powerShowTimeToFull->isChecked());
                }
                connect(cfgDock, &DConfig::valueChanged, powerShowTimeToFull, [cfgDock, powerShowTimeToFull] (const QString &key) {
                    if ("showtimetofull" == key) {
                        powerShowTimeToFull->setChecked(cfgDock->value("showtimetofull").toBool());
                    }
                });
            });
            return powerShowTimeToFull;
        }));

    group->appendChild(new ItemModule("ShowTimeToFullTips", tr("Maximum capacity"),
        [this] (ModuleObject *module) -> QWidget*{
            TitleValueItem *ShowTimeToFullTips = new TitleValueItem();
            ShowTimeToFullTips->setValue(QString::number(int(m_work->getBatteryCapacity())) + "%");
            return ShowTimeToFullTips;
        }));
}

void UseBatteryModule::updateComboxActionList()
{
    m_Options.clear();
    if (m_model->getShutdown()) {
        m_Options.append({ tr("Shut down"), 0 });
    }
    if (m_work->getCurCanSuspend()) {
        m_Options.append({ tr("Suspend"), 1 });
    }
    if (m_work->getCurCanHibernate()) {
        m_Options.append({ tr("Hibernate"), 2 });
    }
    m_Options.append({ tr("Turn off the monitor"), 3 });
    m_Options.append({ tr("Do nothing"), 4 });
}

QString UseBatteryModule::delayToLiteralString(const int delay) const
{
    QString strData = "";

    switch (delay) {
    case 1:
        strData = tr("1 Minute");
        break;
    case 2:
        strData = tr("%1 Minutes").arg(5);
        break;
    case 3:
        strData = tr("%1 Minutes").arg(10);
        break;
    case 4:
        strData = tr("%1 Minutes").arg(15);
        break;
    case 5:
        strData = tr("%1 Minutes").arg(30);
        break;
    case 6:
        strData = tr("1 Hour");
        break;
    case 7:
        strData = tr("Never");
        break;
    default:
        strData = tr("%1 Minutes").arg(15);
        break;
    }

    return strData;
}