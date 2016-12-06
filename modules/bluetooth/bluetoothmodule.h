#ifndef BLUETOOTHMODULE_H
#define BLUETOOTHMODULE_H

#include "moduleinterface.h"

namespace dcc {
namespace bluetooth {

class BluetoothWidget;
class BluetoothModel;
class BluetoothWorker;

class BluetoothModule : public QObject, public ModuleInterface
{
    Q_OBJECT

public:
    explicit BluetoothModule(FrameProxyInterface *frame, QObject *parent = 0);

private:
    ~BluetoothModule();
    void initialize();
    void moduleActive();
    void moduleDeactive();
    void reset();
    void contentPopped(ContentWidget * const w);
    const QString name() const;
    ModuleWidget *moduleWidget();

private:
    BluetoothWidget *m_bluetoothView;
    BluetoothModel *m_bluetoothModel;
    BluetoothWorker *m_bluetoothWorker;
};

}
}

#endif // BLUETOOTHMODULE_H
