# MasterSlaveDBus/CMakeLists.txt

# Добавьте переменные для всех файлов, чтобы использовать их в дальнейшем
set(MASTERSLAVEDBUS_SOURCES
    masterdbus.cpp
    slavedbus.cpp
)

set(MASTERSLAVEDBUS_HEADERS
    masterdbus.h
    slavedbus.h
    servicenameandproperty.h
)

# Создайте библиотеку из исходных файлов
add_library(MasterSlaveDBus ${MASTERSLAVEDBUS_SOURCES} ${MASTERSLAVEDBUS_HEADERS})

# Установите директории включаемых файлов
target_include_directories(MasterSlaveDBus PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

# Привяжите необходимые библиотеки здесь (если есть)
target_link_libraries(MasterSlaveDBus PRIVATE Qt5::Core Qt5::DBus)
