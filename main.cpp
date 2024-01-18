#include <QCoreApplication>
#include <QObject>
#include <QtDBus/QDBusConnection>
#include <QtDBus/QDBusConnectionInterface>
#include <QtDBus/QDBusServiceWatcher>
#include <servicenameandproperty.h>
#include <QDebug>
#include "slavedbus.h"
#include "masterdbus.h"
#include <QTimer>

// Class definition for the main Test class
class Test : public QObject
{
    Q_OBJECT
public:
    // Constructor for Test class
    Test() :  masterDBus(nullptr), slaveDBus(nullptr), timer(this), counter(0) {
        qDebug() << "create";
    }

private slots:
    // Slot to handle timeout for sending MasterDBus messages
    void handleMasterDBusTimeout()
    {
        if (masterDBus)
            masterDBus->sendMsg(QString("test MasterDBus"), counter++);
    }

    // Slot to handle timeout for sending SlaveDBus messages
    void handleSlaveDBusTimeout()
    {
        if(slaveDBus)
            slaveDBus->sendMsg("SlaveDBus send", counter++);
    }

    // Slot to handle the registration of a service
    void handleServiceRegistered(const QString &serviceName)
    {
        if(slaveDBus)
            slaveDBus->connectToService(serviceName);
    }

public:
    // Method to initialize MasterDBus mode
    void initializeMasterDBusMode()
    {
        // Obtain the session D-Bus connection
        QDBusConnection connection = QDBusConnection::sessionBus();

        // Create an instance of the MasterDBus class
        masterDBus = new MasterDBus();

        // Connect the timer timeout signal to the handleMasterDBusTimeout slot
        connect(&timer, &QTimer::timeout, this, &Test::handleMasterDBusTimeout);

        // Connect the MasterDBus class signal haveMsg to a lambda function for logging
        connect(masterDBus, &MasterDBus::haveMsg, this, [](QString str, int num){
            qDebug() << "initializeMasterDBusMode" << str << num;
        });

        // Register the MasterDBus object with the D-Bus
        if (!connection.registerObject("/", masterDBus)) {
            qCritical() << "Can't register object";
            return;
        }

        qDebug() << "MasterDBus connected to D-bus";

        // Register the MasterDBus service with the D-Bus
        if (!connection.registerService(SERVICE_NAME)) {
            qCritical() << "ERR" << QDBusConnection::sessionBus().lastError().message();
            return;
        }

        // Start the timer for sending MasterDBus messages
        timer.start(1000);
        qDebug() << "Test service start";
    }

    // Method to initialize SlaveDBus mode
    void initializeSlaveDBusMode()
    {
        // Create an instance of the SlaveDBus class
        slaveDBus = new SlaveDBus();

        // Connect the timer timeout signal to the handleSlaveDBusTimeout slot
        connect(&timer, &QTimer::timeout, this, &Test::handleSlaveDBusTimeout);

        // Connect the SlaveDBus class signal haveMessage to a lambda function for logging
        connect(slaveDBus, &SlaveDBus::haveMessage, this, [&](QString msg, int num){
            qDebug()<< "SlaveDBus "<< "MESSAGE" << msg << "NUM=" << num;
        });

        qDebug() << "SlaveDBus connected to D-bus";

        // Create a D-Bus service watcher
        QDBusServiceWatcher *watcher = new QDBusServiceWatcher(this);

        // Add the SlaveDBus service to the list of watched services
        watcher->addWatchedService(slaveDBus->m_aviableServiceName);

        // Connect the serviceRegistered signal to the handleServiceRegistered slot
        connect(watcher, &QDBusServiceWatcher::serviceRegistered, this, &Test::handleServiceRegistered);

        // Get a list of registered services and check if the SlaveDBus service is already registered
        QStringList registedServices = QDBusConnection::sessionBus().interface()->registeredServiceNames();

        if (registedServices.contains(slaveDBus->m_aviableServiceName))
            slaveDBus->connectToService(slaveDBus->m_aviableServiceName);

        // Start the timer for sending SlaveDBus messages
        timer.start(1000);
    }

private:
    // Pointers to MasterDBus and SlaveDBus objects
    MasterDBus *masterDBus;
    SlaveDBus *slaveDBus;

    // Timer for periodic message sending
    QTimer timer;

    // Counter for message numbering
    int counter;
};

// Main function
int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    // Create an instance of the Test class
    Test t;

    // Initialize either MasterDBus or SlaveDBus mode based on command line arguments
    if (argc > 1)
        t.initializeMasterDBusMode();
    else
        t.initializeSlaveDBusMode();

    // Start the event loop
    return a.exec();
}
