#pragma once
#include <QMetaType>
#include <QString>
#include <QDBusArgument>

// Define the D-Bus service name
#define SERVICE_NAME "ru.sonarh.dbus.MasterDBus"

// Define the D-Bus buffer name
#define BUFFER_NAME "ru.buffer"

// Define the image data shared ID property
#define IMAGE_DATA_SHARED_ID imageDataSharedId

// Macro for quoting text
#define QUIOTING(text) #text

// Structure to represent test data for D-Bus communication
struct TestStructure {
    int id;
    QString str;
};

// Register the TestStructure type for D-Bus communication
Q_DECLARE_METATYPE(TestStructure)

// Serialization operator for QDBusArgument to serialize TestStructure for D-Bus
inline QDBusArgument& operator <<(QDBusArgument &argument, const TestStructure & arg)
{
    argument.beginStructure();
    argument << arg.id << arg.str;
    argument.endStructure();
    return argument;
}

// Deserialization operator for QDBusArgument to deserialize TestStructure from D-Bus
inline const QDBusArgument& operator >>(const QDBusArgument &argument, TestStructure & arg)
{
    argument.beginStructure();
    argument >> arg.id >> arg.str;
    argument.endStructure();
    return argument;
}
