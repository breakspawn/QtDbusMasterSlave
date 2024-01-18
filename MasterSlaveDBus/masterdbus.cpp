#include "masterdbus.h"
#include <QDebug>
#include <QDBusMetaType>
#include <QDBusConnection>
#include <QDBusMessage>

// MasterDBusAdapter constructor implementation
MasterDBusAdapter::MasterDBusAdapter(MasterDBus *parent) :
    QDBusAbstractAdaptor(parent)
{
    // Set the parent MasterDBus object
    m_parentMasterDBus = parent;

    // Register custom data type TestStructure for D-Bus communication
    qRegisterMetaType<TestStructure>("TestStructure");
    qDBusRegisterMetaType<TestStructure>();
}

// Implementation of imageDataSharedId method in MasterDBusAdapter
QString MasterDBusAdapter::imageDataSharedId()
{
    // Call the corresponding method in the parent MasterDBus object
    return m_parentMasterDBus->imageDataSharedId();
}

// Implementation of structureField method in MasterDBusAdapter
TestStructure MasterDBusAdapter::structureField(QString str, int i)
{
    // Log received parameters and call the corresponding method in the parent MasterDBus object
    qDebug() << str << i;
    return m_parentMasterDBus->structureField();
}

// Implementation of callfromSlaveDBus method in MasterDBusAdapter
void MasterDBusAdapter::callfromSlaveDBus(QString str1, int num)
{
    // Emit the haveMsg signal with received parameters
    emit haveMsg(str1, num);
}

// Implementation of imageDataSharedId method in MasterDBus
MasterDBus::MasterDBus()
{
    m_MasterDBusAdapter = new MasterDBusAdapter(this);
    QObject::connect(this, SIGNAL(sendMsg(QString,int)), m_MasterDBusAdapter, SIGNAL(sendMsg(QString, int)));
    QObject::connect(m_MasterDBusAdapter, SIGNAL(haveMsg(QString, int)), this, SIGNAL(haveMsg(QString, int)));
    m_imageDataSharedId = "testImageBufferId";

}

QString MasterDBus::imageDataSharedId()
{
    // Return the value of the image data shared ID property
    return m_imageDataSharedId;
}

// Counter for the TestStructure ID
int i = 0;

// Implementation of structureField method in MasterDBus
TestStructure &MasterDBus::structureField()
{
    // Log information about the D-Bus message and current TestStructure values
    qDebug() << "Me calld" << QDBusConnection::sessionBus().baseService() << message().service();
    qDebug() << test.str << test.id;

    // Modify the TestStructure and increment the ID counter
    test.str = QString("ku");
    test.id = i++;

    // Return the modified TestStructure
    return test;
}


