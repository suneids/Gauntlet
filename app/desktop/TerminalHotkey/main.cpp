#include <QCoreApplication>
#include <QLocalSocket>
#include <QJsonDocument>
#include <QJsonObject>
#include <QProcess>
#include <QElapsedTimer>

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);

    QLocalSocket socket;
    socket.connectToServer("aggregator");

    if (!socket.waitForConnected(200))
    {
        QProcess::startDetached("x-terminal-emulator");
        return 0;
    }

    QJsonObject request;
    request["type"] = "toggle_terminal";

    socket.write(
        QJsonDocument(request).toJson(QJsonDocument::Compact) + '\n'
        );

    socket.flush();

    QElapsedTimer timer;
    timer.start();

    constexpr int TIMEOUT_MS = 300;

    while (timer.elapsed() < TIMEOUT_MS)
    {
        int remaining = TIMEOUT_MS - timer.elapsed();

        if (!socket.canReadLine())
        {
            if (!socket.waitForReadyRead(remaining))
                break;
        }

        while (socket.canReadLine())
        {
            QByteArray raw = socket.readLine().trimmed();

            QJsonDocument doc = QJsonDocument::fromJson(raw);

            if (!doc.isObject())
                continue;

            QJsonObject obj = doc.object();

            if (obj["type"].toString() == "ack")
                return 0;

            // hide/show/любое другое сообщение просто игнорируем
        }
    }

    QProcess::startDetached("x-terminal-emulator");
    return 0;
}
