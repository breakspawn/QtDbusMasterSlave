#ifndef SlaveDBus_H
#define SlaveDBus_H

#include <QObject>
#include <QDBusAbstractInterface>
#include <qdbusinterface.h>

// Declaration of the SlaveDBus class, which inherits from QObject
class SlaveDBus : public QObject
{
    Q_OBJECT

public:
    // Constructor for the SlaveDBus class, with optional parent parameter
    explicit SlaveDBus(QObject *parent = 0);

public slots:
    // Slot to manage D-Bus connection changes
    void manageConnection(const QString &name, const QString &oldVAlue, const QString &newValue);

    // Slot to connect to a D-Bus service
    void connectToService(const QString &name);

    // Slot to disconnect from a D-Bus service
    void disconnect(const QString &name);

    // Slot to handle messages received from the D-Bus service
    void reacoOnMeCalling(QString message, int num);

public:
    // Method to send a TestStructure to the connected D-Bus service
    void sendStruct(QString str, int num);

    // Method to send a simple message to the connected D-Bus service
    void sendMsg(QString str, int num);

    // Public variable to store the available service name
    QString m_aviableServiceName;

signals:
    // Signal emitted when a message is received from the D-Bus service
    void haveMessage(QString mess1, int mess2);

private:
    // Pointer to the D-Bus interface for communication
    QDBusInterface *m_interface;

    // Name of the D-Bus interface
    QString m_interfaceName;

    // Static constant property name for image data shared ID
    static const QString _propertyName;
};

#endif // SlaveDBus_H
