#ifndef CONFIG_H
#define CONFIG_H

#include <QByteArray>

namespace config
{

const int port = 9000;

enum messageType { message, id, connection, passwordSharing };
enum messageState { none, ok, denied, init, startInit, sendingOpenKey, close };

}

#endif // CONFIG_H
