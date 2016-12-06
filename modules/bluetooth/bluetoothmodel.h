/**
 * Copyright (C) 2016 Deepin Technology Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 **/

#ifndef DCC_BLUETOOTH_BLUETOOTHMODEL_H
#define DCC_BLUETOOTH_BLUETOOTHMODEL_H

#include <QObject>

#include "adapter.h"

namespace dcc {
namespace bluetooth {

class BluetoothModel : public QObject
{
    Q_OBJECT
public:
    explicit BluetoothModel(QObject *parent = 0);

    QList<const Adapter *> adapters() const;

public slots:
    void addAdapter(const Adapter *adapter);
    void removeAdapater(const QString &adapterId);

signals:
    void adapterAdded(const Adapter *adapter) const;
    void adapterRemoved(const QString &adapterId) const;

private:
    QList<const Adapter *> m_adapters;
};

} // namespace bluetooth
} // namespace dcc

#endif // DCC_BLUETOOTH_BLUETOOTHMODEL_H
