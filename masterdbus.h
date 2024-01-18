#ifndef MasterDBus_H
#define MasterDBus_H

#include <QDBusAbstractAdaptor>
#include <QDBusVariant>
#include <QDBusArgument>
#include <QDBusContext>

#include "servicenameandproperty.h"

// Forward declaration to avoid circular dependency
class MasterDBus;

// D-Bus adaptor for the MasterDBus class
class MasterDBusAdapter : public QDBusAbstractAdaptor
{
    Q_OBJECT
    Q_CLASSINFO("D-Bus Interface", BUFFER_NAME)
    Q_PROPERTY(QString IMAGE_DATA_SHARED_ID READ imageDataSharedId)

public:
    explicit MasterDBusAdapter(MasterDBus *parent);

    // D-Bus property getter
    QString imageDataSharedId();

public slots:
    // D-Bus method to provide a structure field
    TestStructure structureField(QString str, int i);

    // D-Bus method to handle calls from SlaveDBus
    void callfromSlaveDBus(QString str1, int num);

signals:
    // Signal emitted when a message is received
    void haveMsg(QString, int);

    // Signal emitted to send a message
    void sendMsg(QString, int);

private:
    MasterDBus *m_parentMasterDBus;  // Pointer to the MasterDBus instance
};

// Main class representing the MasterDBus
class MasterDBus : public QObject, public QDBusContext
{
    Q_OBJECT

public:
    MasterDBus();

public:
    // D-Bus property getter
    QString imageDataSharedId();

    // D-Bus method to provide a structure field
    TestStructure& structureField();

signals:
    // Signal emitted to send a message
    void sendMsg(QString, int);

    // Signal emitted when a message is received
    void haveMsg(QString, int);

private:
    MasterDBusAdapter *m_MasterDBusAdapter;  // Pointer to the MasterDBusAdapter instance
    QString m_imageDataSharedId;             // D-Bus property representing image data
    TestStructure test;                      // Test structure used in D-Bus methods
};

#endif // MasterDBus_H
