#include "slavedbus.h"
#include "servicenameandproperty.h"
#include <QDBusConnectionInterface>
#include <QDebug>
#include <QDBusMetaType>

// Define the property name for image data shared ID
const QString SlaveDBus::_propertyName(QUIOTING(IMAGE_DATA_SHARED_ID));

// Constructor for the SlaveDBus class
SlaveDBus::SlaveDBus(QObject *parent) :
    QObject(parent)
{
    // Check if the session bus is connected
    if (!QDBusConnection::sessionBus().isConnected()) {
        qCritical() << "Cannot connect to the D-Bus session bus.";
        return;
    }

    // Initialize member variables
    m_interface = nullptr;
    m_interfaceName = QString(BUFFER_NAME);
    m_aviableServiceName = QString(SERVICE_NAME);

    // Register custom data type TestStructure for D-Bus communication
    qRegisterMetaType<TestStructure>("TestStructure");
    qDBusRegisterMetaType<TestStructure>();
}

// Slot to manage D-Bus connection changes
void SlaveDBus::manageConnection(const QString& name, const QString &oldVAlue, const QString &newValue)
{
    Q_UNUSED(oldVAlue)

    // Check if the service name matches the available service name
    if(name != m_aviableServiceName)
        return;

    // If the new value is empty, disconnect from the service; otherwise, connect
    if(newValue.isEmpty())
        disconnect(name);
    else
        connectToService(name);
}

// Method to connect to a D-Bus service
void SlaveDBus::connectToService(const QString &name)
{
    // Check if the service name matches the available service name
    if(name != m_aviableServiceName)
        return;

    qDebug()<<"Connceting";

    // Create a new D-Bus interface for communication
    m_interface = new QDBusInterface(name, "/", m_interfaceName, QDBusConnection::sessionBus(), this);

    // Connect the sendMsg signal of the interface to the reacoOnMeCalling slot
    QObject::connect(m_interface, SIGNAL(sendMsg(QString, int)), this, SLOT(reacoOnMeCalling(QString, int)));

    // Check if the interface is valid; otherwise, log an error and delete the interface
    if(!m_interface->isValid()){
        qDebug()<<"Invalid interface"<<m_interface->lastError();
        delete m_interface;
        m_interface = nullptr;
        return;
    }

    qDebug()<<m_interface->interface();

    // Attempt to retrieve the "imageDataSharedId" property from the interface
    QVariant var = m_interface->property("imageDataSharedId");
    qDebug()<<var;

    // Call the "structureField" method on the interface and log the result
    QDBusReply<TestStructure> reply = m_interface->call("structureField");
    QDBusReply<TestStructure> reply1 = m_interface->call("callfromSlaveDBus");

    if(reply.isValid())
    {
        TestStructure testStructure = reply.value();
        qDebug()<<testStructure.id<<testStructure.str;
    }
}

// Slot to disconnect from a D-Bus service
void SlaveDBus::disconnect(const QString &name)
{
    // Check if the service name and interface's service match
    if(name != m_aviableServiceName)
        return;
    if(name != m_interface->service())
        return;

    // Delete the interface and set it to nullptr
    delete m_interface;
    m_interface = nullptr;
    qDebug()<<"Disconnect";
}

// Slot to handle messages received from the D-Bus service
void SlaveDBus::reacoOnMeCalling(QString message, int num)
{
    // Emit the haveMessage signal with the received message and counter
    emit haveMessage(message, num);
}

// Method to send a TestStructure to the connected D-Bus service
void SlaveDBus::sendStruct(QString str, int num)
{
    // If the interface is not valid, reconnect to the service
    if(!m_interface) {
        connectToService(QString(SERVICE_NAME));
        return;
    }

    // Call the "structureField" method on the interface with the provided arguments
    QDBusReply<TestStructure> reply = m_interface->callWithArgumentList(QDBus::CallMode::NoBlock, "structureField", {str, num});

    // If the reply is valid, log the received TestStructure
    if(reply.isValid())
    {
        TestStructure testStructure = reply.value();
        qDebug()<<testStructure.id<<testStructure.str;
    }
}

// Method to send a simple message to the connected D-Bus service
void SlaveDBus::sendMsg(QString str, int num)
{
    // If the interface is not valid, reconnect to the service
    if(!m_interface) {
        connectToService(QString(SERVICE_NAME));
        return;
    }

    // Call the "callfromSlaveDBus" method on the interface with the provided arguments
    QDBusReply<TestStructure> reply = m_interface->callWithArgumentList(QDBus::CallMode::NoBlock, "callfromSlaveDBus", {str, num});
}
