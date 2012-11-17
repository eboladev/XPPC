#include "modemsworkerthread.h"
#include <QEventLoop>
#include <QStringList>
#include <QVariant>
#include <QTimer>
#include <QThread>
#include <QDebug>


ModemsWorkerThread::ModemsWorkerThread(QObject *parent) :
    QObject(parent),
    msecToHangDown(4000)
{
    checkPortThread = new QThread();
    checkPortThread->start();
    checkPortTimer = new QTimer();
    checkPortTimer->start(50);
    checkOutputBuffersTimer = new QTimer();
    checkOutputBuffersTimer->start(50);
    // таймер проверки состояния(наличия) порта помещаем в другой поток
    // потому что при вытаскивании usb to com ftdi под windows перестают
    // работать таймера в потоке, где живет проблемный SerialPort
    checkPortTimer->moveToThread(checkPortThread);
    connect(checkPortTimer, SIGNAL(timeout()), this, SLOT(onCheckPortsTimer()), Qt::QueuedConnection);
    connect(checkOutputBuffersTimer, SIGNAL(timeout()), this, SLOT(onCheckOutputBuffers()));
}

ModemsWorkerThread::~ModemsWorkerThread()
{
    qDebug() << "ModemsWorkerThread";
    QMetaObject::invokeMethod(checkPortTimer, "stop", Qt::BlockingQueuedConnection);
    checkPortThread->quit();
    qDebug() << "ModemsWorkerThread";
    if (!checkPortThread->wait(0x2000))
        checkPortThread->terminate();
    qDebug() << "ModemsWorkerThread";

    delete checkPortThread;
    delete checkPortTimer;
    removeAllModems();
}

bool ModemsWorkerThread::addModem(const QString &modemName, const QString &portName, const QString &telNumber)
{
    // недопустимо создание модема с пустым именем
    if (modemName.isEmpty())
        return false;
    if (modemsList.contains(modemName))
        return false;
    modemInfo mi;
    mi.internalCounter = 0;
    mi.isResponse = true;
    mi.waitingSMSPdu = false;
    mi.status = waitData;
    mi.portName = portName;
    mi.modemName = modemName;
    mi.idSMS = -1;
    mi.networkName = "";
    mi.isInitialized = false;
    mi.isRegisteredInNetwork = false;
    mi.isBusy = true;
    mi.isOpen = false;
    mi.inIncomingRingMode = false;
    mi.isDisapear = false;
    mi.timer = new QTimer();
    mi.timer->setProperty("ModemName", modemName);
    mi.timer->setSingleShot(true);
    mi.buffer = new QByteArray();
    mi.serialPort = new SerialPort();
    mi.serialPort->setProperty("ModemName", modemName);
    modemsList[modemName] = mi;
    // чтение с порта
    connect(mi.serialPort, SIGNAL(readyRead()), this, SLOT(onReadyRead()));
    // таймер для таймаутов
    connect(mi.timer, SIGNAL(timeout()), this, SLOT(onTimeoutWatcherTimer()));
    mi.serialPort->setPort(portName);
    return true;
}

bool ModemsWorkerThread::removeModem(const QString &modemName)
{
    if (!modemsList.contains(modemName))
        return true;
    modemInfo * const pModemInfo = &modemsList[modemName];
    if (modemsList[modemName].isOpen)
    {
        // модем занят отсылкой смс - информуируем об ошибке
        if (pModemInfo->idSMS >= 0)
            emit errorWhileSendingSMS(modemName, pModemInfo->idSMS);
        // если порт открыт - закрываем его не смотря ни на что.
        close(pModemInfo);
        // состояние порта изменилось
        emit portStateChanged(modemName, false);
    }
    delete pModemInfo->buffer;
    delete pModemInfo->serialPort;
    delete pModemInfo->timer;
    modemsList.remove(modemName);
    return true;
}

bool ModemsWorkerThread::closeModem(const QString &modemName)
{
    if (!modemsList.contains(modemName))
        return false;
    modemInfo * const pModemInfo = &modemsList[modemName];
    if (!pModemInfo->isOpen)
        return true;

    // модем занят отсылкой смс - информуируем об ошибке
    if (pModemInfo->idSMS >= 0)
    {
        pModemInfo->idSMS = -1;
        emit errorWhileSendingSMS(modemName,pModemInfo->idSMS);
    }
    // состояние порта изменилось
    emit portStateChanged(modemName, false);
    close(pModemInfo);
    return true;
}

bool ModemsWorkerThread::sendSMS(const QString &smsText, const QString &phoneNumber, const int &idSMS)
{
    foreach (const QString &modemName, modemsList.keys())
    {
        if (modemsList.value(modemName).isRegisteredInNetwork)
        {
            sms s;
            s.smsId = idSMS;
            s.smsTest = smsText;
            s.telephoneNumber = phoneNumber;
            smsList.append(s);
            return true;
        }
    }
    return false;
}

void ModemsWorkerThread::removeAllModems()
{
    QList<QString> modems = modemsList.keys();
    foreach (const QString &modemName, modems) {
        removeModem(modemName);
    }
}

bool ModemsWorkerThread::changeModemName(const QString &oldModemName, const QString &newModemName)
{
    if (!modemsList.contains(oldModemName))
        return false;
    if (modemsList.contains(newModemName))
        return false;
    modemInfo *pModemInfo = &modemsList[oldModemName];
    pModemInfo->modemName = newModemName;
    pModemInfo->timer->setProperty("ModemName", newModemName);
    pModemInfo->serialPort->setProperty("ModemName", newModemName);
    modemsList[newModemName] = *pModemInfo;
    modemsList.remove(oldModemName);
    return true;
}

void ModemsWorkerThread::addInitializeCommand(QStringList &stringList)
{
    /* отключаем нафиг эхо */
    stringList.append("ATE0");
    /* Команда настройки, куда помещать вх. СМС и отчеты о доставке.
     * В данном случае и то и то сразу в терминал, в память не записывать */
    stringList.append("AT+CNMI=2,2,0,1,0");
    /* Команда на включение АОНа */
    stringList.append("AT+CLIP=1");
    /* Команда на переход в PDU режим */
    stringList.append("AT+CMGF=0");
    // команда на проверку регистрации в сети
    stringList.append("AT+CREG?");
    // уровень сигналоа
    stringList.append("AT+CSQ");
    // название оператора
    stringList.append("AT+COPS?");
}

QString ModemsWorkerThread::getPDUText(QString telNum, QString smsText)
{
    /* проверяем СМС на наличие в нем не ASCII символов, и
     * соответственно выбираем формат кожирования текста СМС
     * (0 - 7 бит ASCII, 8 - UCS2 (UTF-16) ) */
    char textFormat = 0;
    for (int i = 0; i < smsText.length(); ++i)
        if (smsText.at(i) >= 128)
        {
            textFormat = 8;
            // максимум 70 символов для кирилицы
            smsText = smsText.left(70);
            break;
        }
    // максимум 160 символов для ascii
    if (textFormat == 0)
        smsText = smsText.left(160);

    /* Международный или местный формат телефонного номера */
    QString telephoneNumberFormat = "81";
    if (telNum.at(0) == QChar('+'))
    {
        telNum.remove(0, 1);
        telephoneNumberFormat = "91";
    }

    /* подготавливает телефонный номер к кодировке */
    int telNumLength = telNum.length();

    if (telNum.length() % 2 == 1)
        telNum.append("F");
    for (int i=0; i < telNum.length() / 2; ++i)
    {
        QChar x = telNum.at(i * 2);
        telNum[i * 2] = telNum.at(i * 2 + 1);
        telNum[i *2 + 1] = x;
    }

    // Длина и номер SMS центра. 0 - означает, что будет использоваться дефолтный номер.
    QString pdu = "00";

    // SMS-SUBMIT
    pdu.append("31");
    // Длина и номер отправителя. 0 - означает что будет использоваться дефолтный номер.
    pdu.append("00");
    // Длина номера получателя
    pdu.append(QString("%0").arg(telNumLength, 2, 16, QChar('0')).toUpper());
    // Тип-адреса. (91 указывает международный формат телефонного номера, 81 - местный формат).
    pdu.append(telephoneNumberFormat);
    // Телефонный номер получателя в международном формате.
    pdu.append(telNum);
    // Идентификатор протокола
    pdu.append("00");
    // Старший полубайт означает сохранять SMS у получателя или нет (Flash SMS),  Младший полубайт - кодировка(0-латиница 8-кирилица).
    pdu.append("0");
    pdu.append('0' + textFormat);
    // Срок доставки сообщения. С1 - неделя
    pdu.append("C1");

    if (textFormat == 8)
    {
        // содержит non latin chars
        // Длина текста сообщения.
        pdu.append(QString("%0").arg(smsText.length() * 2, 2, 16, QChar('0')).toUpper());
        const ushort *pSmsText = smsText.utf16();
        int i = 0;
        while(*(pSmsText + i))
        {
            pdu.append(QString("%0").arg((*(pSmsText + i)) / 0x100 , 2, 16, QChar('0')).toUpper());
            pdu.append(QString("%0").arg((*(pSmsText + i)) % 0x100 , 2, 16, QChar('0')).toUpper());
            ++i;
        }
    } else
    {
        // только латинские символы. Упаковываем в 7 бит.
        // Длина текста сообщения.

        int lenght = smsText.length() * 7 / 8 + ((smsText.length() * 7) % 8 == 0 ? 0 : 1);
        pdu.append(QString("%0").arg(smsText.length(), 2, 16, QChar('0')).toUpper());
        QByteArray ba(lenght, 0x00);
        int numBit = 1;
        int numByte = 0;
        for (int i = 0; i < smsText.length(); ++i)
        {
            uchar symb = smsText.at(i).toLatin1();
            uchar nexsymb = 0;
            if (i + 1 < smsText.length())
                nexsymb = smsText.at(i + 1).toLatin1();
            symb = symb >> (numBit - 1);
            uchar mask = (0x01 << numBit) - 1;
            nexsymb = nexsymb & (mask);
            nexsymb = nexsymb << (8 - (numBit   ));
            symb = symb | nexsymb;
            ba[numByte] = symb;
            if (numBit == 7)
            {
                numBit = 0;
                ++i;
            }
            ++numByte;
            ++numBit;
        }
        for (int i = 0; i < ba.length(); ++i)
            pdu.append(QString("%0").arg((quint8)ba.at(i), 2, 16, QChar('0')).toUpper());
    }
    return pdu;
}

void ModemsWorkerThread::close(modemInfo *pModemInfo)
{
    pModemInfo->isOpen = false;
    pModemInfo->isDisapear = false;
    pModemInfo->isBusy = false;
    pModemInfo->isInitialized = false;
    pModemInfo->isRegisteredInNetwork = false;
    pModemInfo->idSMS = -1;
    pModemInfo->timer->stop();
    pModemInfo->serialPort->reset();
    pModemInfo->serialPort->close();
}

bool ModemsWorkerThread::open(modemInfo *pModemInfo)
{
    if (pModemInfo->serialPort->isOpen())
        return true;
    pModemInfo->serialPort->setPort(pModemInfo->portName);
    bool isOpen = pModemInfo->serialPort->open(SerialPort::ReadWrite);
    if (isOpen)
    {
        pModemInfo->serialPort->setDataBits(SerialPort::Data8);
        pModemInfo->waitingSMSDeliveryPDU = false;
        pModemInfo->waitingSMSPdu = false;
        pModemInfo->serialPort->setFlowControl(SerialPort::NoFlowControl);
        pModemInfo->serialPort->setParity(SerialPort::NoParity);
        pModemInfo->serialPort->setStopBits(SerialPort::OneStop);
        pModemInfo->serialPort->setRate(SerialPort::Rate19200);
        pModemInfo->isOpen = isOpen;
        pModemInfo->initializeCommands.clear();
        pModemInfo->idSMS = -1;
        pModemInfo->isBusy = false;
        pModemInfo->isDisapear = false;
        pModemInfo->isRegisteredInNetwork = false;
        pModemInfo->currentBufferPosition = 0;
        pModemInfo->buffer->clear();
        addInitializeCommand(pModemInfo->initializeCommands);
    }
    return isOpen;
}

QString ModemsWorkerThread::getASCIIFrom7BitCodedString(const QString &str)
{
    char bitN = 7;
    uchar temp = 0;
    QString smsText;
    for (int i = 0; i < str.length() / 2; ++i)
    {
        uchar byte = str.mid(i * 2, 2).toInt(0, 16);
        uchar cyrS = (temp | (byte << (7 - bitN))) & 0x7f;
        temp = byte >> bitN;
        if (cyrS != 0 && cyrS <= 127 ) smsText.append(QChar(cyrS));
        --bitN;
        if (bitN == 0)
        {
            if (temp != 0 && temp <= 127 ) smsText.append(QChar(temp));
            bitN = 7;
            temp = 0;
        }
    }
    return smsText;
}

void ModemsWorkerThread::onReadyRead()
{


    // определяем конец строки раз и навсегда
    static const QByteArray endOfLine("\r\n");
    if (!sender())
        return;
    QString modemName = sender()->property("ModemName").toString();
    // свойство не установлено
    if (modemName.isEmpty())
        return;
    // не получилось найти модем с таким именем
    if (!modemsList.contains(modemName))
        return;

    modemInfo *pModemInfo = &modemsList[modemName];
    pModemInfo->isBusy = true;
    pModemInfo->timer->stop();

    if (pModemInfo->status == waitPeriodForReinitialize)
    {
        pModemInfo->status = waitData;
        pModemInfo->isBusy = false;
        pModemInfo->initializeCommands.clear();
        addInitializeCommand(pModemInfo->initializeCommands);
    }
    // пришли данные = помечаем модем как занятый

    if (!pModemInfo->isResponse){
        pModemInfo->isResponse = true;
        emit isModemResponse(pModemInfo->modemName, true);
    }
    // прочитали то, что пришло от модема
    // и дополнили буфер
    QByteArray arr = pModemInfo->serialPort->readAll();
    pModemInfo->buffer->append(arr);
    // попытка парсить вывод ответа модема
    int bufferLenght = pModemInfo->buffer->length();

    int sizeOfPacket = bufferLenght - pModemInfo->currentBufferPosition;

    if (sizeOfPacket <= 0)
    {
        // херня какая-то, дочитать
        pModemInfo->timer->setSingleShot(true);
        pModemInfo->timer->start(1000);
    }
    if (bufferLenght > 0x1000)
    {
        // если буфер разросся больше 4к - уменьшим его
        pModemInfo->buffer->remove(0,0x0f00);
        // скорректировали переменные
        bufferLenght -= 0x0f00;
        pModemInfo->currentBufferPosition -= 0x0f00;
    }

    if (pModemInfo->status == waitSmsResponse1)
    {
        qDebug() << "wait Sms:" << pModemInfo->buffer->data() + pModemInfo->currentBufferPosition;
        // первый отклик при посылке смс;
        if (pModemInfo->buffer->indexOf("> ", pModemInfo->currentBufferPosition) == -1)
        {
            // беда. пытаемся дождать.
            pModemInfo->timer->setSingleShot(true);
            pModemInfo->timer->start(200);
            return;
        }
        // если всё хорошо - подготавливаем вторую часть посылки смс
        pModemInfo->serialPort->write(pModemInfo->currentPduForSms.toLatin1().append(0x1a));
        // при отправке смс ставим таймер на побольше
        pModemInfo->timer->setSingleShot(true);
        pModemInfo->timer->start(sendSmsTimeout);
        pModemInfo->status = waitSmsResponse2;
        return;
    }

    // подсчет, за сколько проходов формируется строка
    // для отладочных целей
    static int count = 0;
    while (sizeOfPacket > 0)
    {
        ++count;
        int idx = pModemInfo->buffer->right(sizeOfPacket).indexOf(endOfLine);
        if (idx >= 0)
        {
            QString str = QString(pModemInfo->buffer->mid(pModemInfo->currentBufferPosition, idx)).trimmed();

            pModemInfo->currentBufferPosition += idx;
            pModemInfo->currentBufferPosition +=2;
            sizeOfPacket -= idx;
            sizeOfPacket -= 2;
            // пустую строку парсить нет смысла
            if (str.isEmpty())
                continue;
            qDebug() << "command" << str << pModemInfo->status << count;
            switch(pModemInfo->status)
            {
            case waitData:
            {
                if (parseCommonAnswers(str, pModemInfo))
                    break;
                if (parseSimpleAnswert(str, pModemInfo))
                    break;
            }
                break;
            case waitSmsResponse1:
            break;
            case waitSmsResponse2:
            {
                // здесь вытаскивать номер внутренний номер смс
                // вначале проверка на ошибку

                // потом проверка на всякую байду
                if (parseCommonAnswers(str, pModemInfo))
                {
                    pModemInfo->status = waitSmsResponse2;
                    // пытаемся получить  инфу о смс еще раз
                    pModemInfo->timer->setSingleShot(true);
                    pModemInfo->timer->start(sendSmsTimeout);
                    break;
                }
                if (parseErrorResponse(str, pModemInfo))
                {
                    //Ошибка :-(
                    emit errorWhileSendingSMS(pModemInfo->modemName, pModemInfo->idSMS);
                    pModemInfo->status = waitData;
                    pModemInfo->idSMS = -1;
                    break;
                }
                //
                QRegExp regExp("\\+CMGS: ?(\\d+)\\s*");
                if (regExp.indexIn(str, 0) >= 0)
                {
                    pModemInfo->status = waitSimpleResponse;
                    int smsId = regExp.cap(1).toInt();
                    emit smsSended(pModemInfo->modemName, pModemInfo->idSMS);
                    pModemInfo->smsPendingTheReceipt[smsId] = pModemInfo->idSMS;
                    pModemInfo->idSMS = -1;
                    break;
                }

                // вообще не совпало ни с чем. сл. попытка дождаться ответ.
                pModemInfo->status = waitSmsResponse2;
                pModemInfo->timer->setSingleShot(true);
                pModemInfo->timer->start(commandsTimeout);
            }
                break;
            case waitInitializeCommandResponse:
            case waitSimpleResponse:
            {
                if (parseSimpleAnswert(str, pModemInfo))
                {
                    pModemInfo->status = waitData;
                    break;
                }
                parseCommonAnswers(str, pModemInfo);

                // надо дождаться ОК или ERROR
                pModemInfo->timer->setSingleShot(true);
                pModemInfo->timer->start(commandsTimeout);
            }
                break;
            case waitPeriodForReinitialize:
            {

            }
                break;
            }

            //pModemInfo->isBusy = false;
            count = 0;
        } else
        {
            // хер, а не строка
            sizeOfPacket = -1;
        }
    }
}

void ModemsWorkerThread::onCheckPortsTimer()
{
    foreach(const QString &modemName, modemsList.keys())
    {
        modemInfo * const pModemInfo = &modemsList[modemName];

        if (pModemInfo->isDisapear == false)
        {
            // проверка наличия порта
            if (pModemInfo->isOpen)
            {
                SerialPortInfo si(*pModemInfo->serialPort);
                if (!si.isValid())
                {
                    // что то с портом не так
                    if (pModemInfo->idSMS >= 0)
                    {
                        emit errorWhileSendingSMS(modemName, pModemInfo->idSMS);
                        pModemInfo->idSMS = -1;
                    }
                    close(pModemInfo);
                    pModemInfo->isDisapear = true;
                    emit portIsLost(modemName);
                    qDebug() << "port" << modemName <<"is lost";
                }
            }
        } else
        {
            // попытка востановить состояние порта
            SerialPortInfo si(pModemInfo->portName);
            if (si.isValid())
            {
                // ух ты, появился порт!
                // пробуем открыть
                if (open(pModemInfo))
                {
                    // получилось!
                    emit portIsRestored(modemName);
                    qDebug() << "port" << modemName <<"is restored";
                }
            }
        }
    }
}

void ModemsWorkerThread::onTimeoutWatcherTimer()
{
    qDebug() << "on timeout";
    if (!sender())
        return;
    QString modemName = sender()->property("ModemName").toString();
    if (modemName.isEmpty())
        return;
    if (!modemsList.contains(modemName))
        return;
    modemInfo * const pModemInfo = &modemsList[modemName];

    // ну что ж, произошло страшное - таймаут для модема modemName
    bool tryToReinizializeModem = true;
    switch(pModemInfo->status)
    {
    case waitData:
    {
        tryToReinizializeModem = false;
        pModemInfo->isBusy = false;
    }
        break;
    case waitSimpleResponse:
    {

    }
        break;
    case waitSmsResponse1:
    {
        // таймаут при посылке СМС
        qDebug() << "timeout 1";
        emit errorWhileSendingSMS(modemName, pModemInfo->idSMS);
        pModemInfo->idSMS = -1;
        pModemInfo->serialPort->write(QByteArray().append(0x1a));
    }
        break;
    case waitSmsResponse2:
    {
        // таймаут при посылке СМС
        qDebug() << "timeout 2";
        emit errorWhileSendingSMS(modemName, pModemInfo->idSMS);
        pModemInfo->idSMS = -1;
    }
        break;
    case waitInitializeCommandResponse:
    {

    }
        break;
    case waitPeriodForReinitialize:
    {
        pModemInfo->initializeCommands.clear();
        pModemInfo->status = waitData;
        pModemInfo->isBusy = false;
        addInitializeCommand(pModemInfo->initializeCommands);
        tryToReinizializeModem = false;
    }
        break;
    }
    // если нужно переинициализиовать модем - ждем 20 сеунд
    // и снова пытаемся проинициализировать модем
    if (tryToReinizializeModem)
    {
        emit isModemResponse(pModemInfo->modemName, false);
        pModemInfo->isResponse = false;
        pModemInfo->status = waitPeriodForReinitialize;
        pModemInfo->isBusy = true;
        pModemInfo->isRegisteredInNetwork = false;
        pModemInfo->isInitialized = false;
        pModemInfo->serialPort->reset();
        pModemInfo->timer->start(waitAfterTimeoutBeforeReinitialize);
    }
}

void ModemsWorkerThread::onCheckOutputBuffers()
{
    foreach(const QString &modemName, modemsList.keys())
    {
        modemInfo * const pModemInfo = &modemsList[modemName];
        // форсированый запрос статуса регистрации в сети каждые 2 секунды
        // если модем в сети не зарегистрирован
        if (!pModemInfo->isRegisteredInNetwork)
            if (!(pModemInfo->internalCounter % 10))
                pModemInfo->simpleCommands.append("AT+CREG?");

        // переодически вызываем команды, инициализирующие модем
        // и запрашиающие его статус
        if (pModemInfo->internalCounter == 50)
            pModemInfo->simpleCommands.append("AT+CLIP=1");
        else if (pModemInfo->internalCounter == 100)
            pModemInfo->simpleCommands.append("AT+CNMI=2,2,0,1,0");
        else if (pModemInfo->internalCounter == 150)
            pModemInfo->simpleCommands.append("AT+CMGF=0");
        else if (pModemInfo->internalCounter == 200)
            pModemInfo->simpleCommands.append("AT+CSQ");
        else if (pModemInfo->internalCounter == 250)
            pModemInfo->simpleCommands.append("AT+COPS?");
        else if (pModemInfo->internalCounter == 300)
        {
            pModemInfo->simpleCommands.append("AT+CREG?");
            pModemInfo->internalCounter = 0;
        }
        ++(pModemInfo->internalCounter);

        if (pModemInfo->initializeCommands.isEmpty() && pModemInfo->isOpen && !pModemInfo->isBusy && !pModemInfo->isInitialized)
        {
            qDebug() << "modem initialized";
            pModemInfo->isInitialized = true;
            pModemInfo->status = waitData;
        }
        if (!pModemInfo->initializeCommands.isEmpty() && !pModemInfo->isBusy && pModemInfo->isOpen)
        {

            // команды на инициализацию модема
            pModemInfo->isBusy = true;
            pModemInfo->status = waitInitializeCommandResponse;
            pModemInfo->serialPort->write(pModemInfo->initializeCommands.at(0).toLatin1().append(0x0d));
            pModemInfo->lastGSMCommand = pModemInfo->initializeCommands.at(0);
            qDebug() << "write in port" << pModemInfo->initializeCommands.at(0);
            pModemInfo->initializeCommands.removeFirst();
            pModemInfo->timer->setSingleShot(true);
            pModemInfo->timer->start(commandsTimeout);
            continue;
        } else
        if (!pModemInfo->isBusy && pModemInfo->isOpen && pModemInfo->isInitialized)
        {
            // нашли свободный модем
            // сначала проверим, есть ли команды на отправку текущие

            if (pModemInfo->initializeCommands.isEmpty() && !pModemInfo->isInitialized)
                pModemInfo->isInitialized = true;
            if (!pModemInfo->simpleCommands.isEmpty() && pModemInfo->status == waitData)
            {
                // есть какие-то простые команды
                // пихаем их в последовательный порт
                qDebug() << "write in port" << pModemInfo->simpleCommands.at(0);
                pModemInfo->isBusy = true;
                pModemInfo->lastGSMCommand = pModemInfo->simpleCommands.at(0);
                pModemInfo->serialPort->write(pModemInfo->simpleCommands.at(0).toLatin1().append(0x0d).append(0x0a));
                pModemInfo->status = waitSimpleResponse;
                // удаляем эту команду
                pModemInfo->simpleCommands.removeFirst();
                // время таймаута
                pModemInfo->timer->start(commandsTimeout);
                //переходим к следующему модему
                continue;
            } else if (!pModemInfo->complexCommands.isEmpty() && pModemInfo->status == waitData)
            {
                pModemInfo->isBusy = true;
                pModemInfo->serialPort->write(pModemInfo->complexCommands.at(0).toLatin1().append(0x0d).append(0x0a));
                pModemInfo->status = waitSimpleResponse;
                // удаляем эту команду
                pModemInfo->complexCommands.removeFirst();
                // время таймаута
                pModemInfo->timer->setSingleShot(true);
                pModemInfo->timer->start(commandsTimeout);
                //переходим к следующему модему
                continue;
            } else if (pModemInfo->isRegisteredInNetwork && !smsList.isEmpty() && pModemInfo->status == waitData){
                // так. В этом блоке, если мы зарегистрированы в сети.
                // можно отправлять СМС.
                /* информируем о начале передачи СМС */
                emit startSendingSMS(modemName, pModemInfo->idSMS);
                sms s = smsList.at(0);
                smsList.removeFirst();
                pModemInfo->isBusy = true;
                pModemInfo->currentSms = s.smsTest;
                pModemInfo->idSMS = s.smsId;
                pModemInfo->phoneOfRecipient = s.telephoneNumber;
                pModemInfo->status = waitSmsResponse1;
                pModemInfo->currentPduForSms = getPDUText(pModemInfo->phoneOfRecipient, pModemInfo->currentSms);
                /* формируем и запоминаем команду на отправку смс */
                QString command = QString("AT+CMGS=%0").arg((pModemInfo->currentPduForSms.length() - 2)/2);
                /* кидаем ее в модем */
                pModemInfo->serialPort->write(command.toLatin1().append(0x0d));
                pModemInfo->timer->setSingleShot(true);
                pModemInfo->timer->start(commandsTimeout);
                // запускаем таймер на таймаут
            }
        }
    }
}

void ModemsWorkerThread::onHangDownTimer()
{
    if (!sender())
        return;
    QString modemName = sender()->property("ModemName").toString();
    // свойство не установлено
    if (modemName.isEmpty())
        return;
    // не получилось найти модем с таким именем
    if (!modemsList.contains(modemName))
        return;
    modemInfo * const pModemInfo = &modemsList[modemName];
    pModemInfo->serialPort->write(QByteArray("ATH").append(0x0d));
    pModemInfo->inIncomingRingMode = false;
    // больше нам этот таймер не нужен
    sender()->deleteLater();
    pModemInfo->timer->setSingleShot(true);
    pModemInfo->timer->start(commandsTimeout);
}


bool ModemsWorkerThread::openModem(const QString &modemName)
{
    if (!modemsList.contains(modemName))
        return false;
    modemInfo * const pModemInfo = &modemsList[modemName];
    if (pModemInfo->isOpen)
        return true;
    pModemInfo->serialPort->setPort(pModemInfo->portName);
    bool isOpen = open(pModemInfo);
    if (isOpen)
    {
        emit portStateChanged(modemName, isOpen);
    }
    return isOpen;
}


bool ModemsWorkerThread::regExpHelper(const QString &regExp, const QString &str, int &error)
{
    QRegExp re(regExp);
    if (re.indexIn(str) < 0)
        return false;
    bool ok;
    error=re.cap(1).toInt(&ok);
    return ok;
}

bool ModemsWorkerThread::containsCMEError(const QString &textLine, int &error)
{
    QString regExp = "CME ERROR:\\s?(\\d+)\\s*\\x0D";
    return regExpHelper(regExp, textLine, error);
}

bool ModemsWorkerThread::containsCMSError(const QString &textLine, int &error)
{
    QString regExp = "CMS ERROR:\\s?(\\d+)\\s*$";
    return regExpHelper(regExp, textLine, error);
}

bool ModemsWorkerThread::parseOkResponse(const QString &textLine, modemInfo *const pModemInfo)
{
    if (textLine.contains(QRegExp("^OK\\s*$")))
    {
        pModemInfo->timer->stop();
        pModemInfo->isBusy = false;
        return true;
    }
    else
        return false;
}

bool ModemsWorkerThread::parseErrorResponse(const QString &textLine, modemInfo *const pModemInfo)
{
    if (textLine.contains("ERROR"))
    {
        pModemInfo->timer->stop();
        pModemInfo->isBusy = false;
        int errorCode;
        if (containsCMEError(textLine, errorCode))
        {
            emit CMEError(pModemInfo->modemName, errorCode);
            return true;
        } else if (containsCMSError(textLine, errorCode))
        {
            emit CMSError(pModemInfo->modemName, errorCode);
            return true;
        }
        emit error(pModemInfo->modemName, pModemInfo->lastGSMCommand);
        return true;
    }
    return false;
}

bool ModemsWorkerThread::parseIncomingRing(const QString &textLine, modemInfo *const pModemInfo)
{
    if (textLine.contains(QRegExp("^RING$")))
        return true;
    QRegExp re("^\\+CLIP:\\s*\"(\\+?\\d+)\"");
    int pos = re.indexIn(textLine, 0);
    if (pos >= 0)
    {
        // есть какой-то номер
        QString number = re.cap(1);
        pModemInfo->lastIncomingRingNumber = number;
        if (!pModemInfo->inIncomingRingMode)
        {
            // первый звонок
            pModemInfo->inIncomingRingMode = true;
            emit incomingRing(pModemInfo->modemName, pModemInfo->lastIncomingRingNumber);
            QTimer *timer = new QTimer();
            connect(timer, SIGNAL(timeout()), this, SLOT(onHangDownTimer()));
            timer->setSingleShot(true);
            timer->setProperty("ModemName", pModemInfo->modemName);
            timer->start(msecToHangDown);
        }


        return true;
    }
    return false;

}


bool ModemsWorkerThread::parseRegisterInNetwork(const QString &textLine, modemInfo *const pModemInfo)
{
    QRegExp re("^\\+CREG: *\\d+ *,\\s*(\\d+)\\s*$");
    if (re.indexIn(textLine, 0) >= 0)
    {
        int status = re.cap(1).toInt();
        emit isRegisteredInNetwork(pModemInfo->modemName, status);
        if (status == 1 || status == 5)
            pModemInfo->isRegisteredInNetwork = true;
        else
            pModemInfo->isRegisteredInNetwork = false;
        return true;
    }
    return false;
}

bool ModemsWorkerThread::parseDeliveryReport(const QString &textLine, modemInfo *const pModemInfo)
{
    if (!pModemInfo->waitingSMSDeliveryPDU)
    {
        QRegExp re("^\\+CDS: *(\\d+)\\s*");
        if (re.indexIn(textLine, 0) >= 0)
        {
            pModemInfo->waitingSMSDeliveryPDU = true;
            return true;
        }
        return false;
    }

    // хочешь не хочешь, а модем надо "освобождать"
    pModemInfo->isBusy = false;
    QRegExp re("([\\dA-F]+)");
    pModemInfo->waitingSMSDeliveryPDU = false;
    if (re.indexIn(textLine, 0) >= 0)
    {
        /* пришёл отчет о доставке СМС*/
        if (textLine.length() < 8)
            return false;
        QString smsPDU = re.cap(1);
        int index = 1;
        bool  ok;
        int byte = smsPDU.mid(0,2).toInt(&ok, 16);
        if (!ok)
            return false;
        /* пропустили номер СЦ */
        index += byte;

        /*  флаги так же пропускаем, так как по умолчанию здесь отчет о доставке */
        ++index;

        if (textLine.length() < (index * 2 + 2))
            return false;

        byte = smsPDU.mid(index * 2, 2).toInt(&ok, 16);
        /* получили номер смс */

        if (!pModemInfo->smsPendingTheReceipt.contains(byte))
            return true;
        int smsId = pModemInfo->smsPendingTheReceipt.value(byte);
        pModemInfo->smsPendingTheReceipt.remove(smsId);
        emit smsDelivered(pModemInfo->modemName, smsId, true);
        return true;
    }
    return false;
}

bool ModemsWorkerThread::parseCommonAnswers(const QString &textLine, modemInfo *const pModemInfo)
{
    if (parseIncomingSMS(textLine, pModemInfo))
        return true;
    if (parseIncomingRing(textLine, pModemInfo))
        return true;
    if (parseRegisterInNetwork(textLine, pModemInfo))
        return true;
    if (parseDeliveryReport(textLine, pModemInfo))
        return true;
    if (parseSignalLevel(textLine, pModemInfo))
        return true;
    if (parseOperatorName(textLine, pModemInfo))
        return true;
    return false;
}

bool ModemsWorkerThread::parseSimpleAnswert(const QString &textLine, modemInfo *const pModemInfo)
{
    if (parseOkResponse(textLine, pModemInfo))
        return true;
    if (parseErrorResponse(textLine, pModemInfo))
        return true;
    return false;
}

bool ModemsWorkerThread::parseIncomingSMS(const QString &textLine, modemInfo *const pModemInfo)
{
    QRegExp re;
    if (pModemInfo->waitingSMSPdu)
    {
        // "освобождаем" модем
        pModemInfo->isBusy = false;
        pModemInfo->waitingSMSPdu = false;
        //!! точно должна быть смска!!!
        re.setPattern("^([\\dA-F]+)$");
        if (re.indexIn(textLine, 0) >= 0)
        {
            QString smsPDU = re.cap(1);
            QString telNumber, smsText;
            if (parsePDU(smsPDU, telNumber, smsText))
            {
                emit incomingSMS(pModemInfo->modemName, telNumber, smsText);
                qDebug() << "Incoming sms:" << telNumber << smsText;
            }
        }
        return false;
    }
    re.setPattern("^\\+CMT:\\s*(?:\".*\"\\s*,)\\s*(\\d+)");
    if (re.indexIn(textLine, 0) >= 0)
    {
        //совпало
        pModemInfo->waitingSMSPdu = true;
        return true;
    }
    return false;
}

bool ModemsWorkerThread::parseOperatorName(const QString &textLine, modemInfo *const pModemInfo)
{
    QRegExp re("^\\+COPS:.*\"(.*)\"");
    if (re.indexIn(textLine, 0) >= 0)
    {
        pModemInfo->networkName = re.cap(1);
        emit operatorName(pModemInfo->modemName, re.cap(1));
        return true;
    }
    return false;
}

bool ModemsWorkerThread::parseSignalLevel(const QString &textLine, modemInfo *const pModemInfo)
{
    QRegExp re ("^\\+CSQ: *(\\d+) *, *(\\d+)");
    if (re.indexIn(textLine,0) >= 0)
    {
        /* извлекаем уровни сигнала и ошибок */
        QString signalLevel = re.cap(1);
        QString errorLevel = re.cap(2);
        pModemInfo->m_rssi = signalLevel.toInt();
        pModemInfo->m_bor = errorLevel.toInt();

        /* информируем о них */
        emit signalLevels(pModemInfo->modemName, pModemInfo->m_rssi, pModemInfo->m_bor);
        return true;
    }
    return false;
}

bool ModemsWorkerThread::parsePDU(const QString &smsPDU, QString &_telephone, QString &_smsText)
{
    bool ok;
    int index = 1;
    if (smsPDU.length() < 8)
        return false;

    /* длинна номера СЦ */
    int byte = smsPDU.mid(0,2).toInt(&ok, 16);

    if (!ok)
        return false;

    /* пропускаем номер сервисного центра */
    index += byte;
    if (smsPDU.length() < (index * 2 + 2))
        return false;

    /* взяли TP-MTI & Co */
    quint8 tp_mti = smsPDU.mid(index * 2, 2).toInt(&ok, 16);
    Q_UNUSED(tp_mti)

    /* перешли на байт длинны номера отправителя */
    ++index;

    /* Взяли эту длинну */
    int lenSenderAdress = smsPDU.mid(index * 2, 2).toInt(&ok, 16);

    /* переходим на формат номера */
    ++index;

    /* берем ее */
    quint8 formatSenderAdress = smsPDU.mid(index * 2, 2).toInt(&ok, 16);
    if (smsPDU.length() < (index * 2 + 2))
        return false;

    /* нам интересны только форматы
     * 0x91 - международный, префикс "+",
     * 0xA1, 0x*1 - местный, без префикса
     * 0xD0 - текстовый, 7 битная кодировка
     */

    /*переходим на начало номера */
    ++index;
    QString telNumber;
    if (formatSenderAdress == 0x91)
        telNumber = "+";
    if (formatSenderAdress == 0x91 || formatSenderAdress == 0xA1 || formatSenderAdress == 0x81)
    {
        /* здесь - если цифровой номер */
        int x = index * 2;
        if (smsPDU.length() < (x + (lenSenderAdress + lenSenderAdress % 2)))
            return false;
        for (int i = 0; i < lenSenderAdress/2; ++i)
        {

            telNumber.append(smsPDU.at(x + i*2 + 1));
            telNumber.append(smsPDU.at(x + i*2));
        }
        if (lenSenderAdress % 2)
        {
            /* нечетный номер */
            telNumber.append(smsPDU.at(x + lenSenderAdress + 1));
        }
    } else if (formatSenderAdress == 0xD0)
    {
        /* здесь - если буквенно-цифровой в 7-ми битной кодировке. */
        //TODO: распарсить и выдать
        int x = index * 2;
        lenSenderAdress += lenSenderAdress % 2;
        if (smsPDU.length() < (x + lenSenderAdress))
            return false;
        QString temp = smsPDU.mid(x, lenSenderAdress);
        telNumber = getASCIIFrom7BitCodedString(temp);
    } else
        telNumber = trUtf8("номер не известен");

    _telephone = telNumber;

    /* переходим к TP-PID */
    index += lenSenderAdress / 2;
    index += lenSenderAdress % 2;

    /* пропускаем и переходим к TD-DCS (формат сообщения) */
    ++index;
    if (smsPDU.length() < (index * 2 + 2))
        return false;

    int tp_dcs = smsPDU.mid(index * 2, 2).toInt(&ok, 16);
    /* переходим к TP-SCTS - штампу времени оператора */
    ++index;

    /* и пропускаем его и TP-UDL */
    index += 8;

    /* здесь поле данных пользователя, извлекаем его */
    if (smsPDU.length() < (index * 2 + 2))
            return false;
    QString tp_ud = smsPDU.mid(index * 2);
    QString smsText = trUtf8("не удалось получить текст сообщения");
    if ((tp_dcs & 0x0f) == 8)
    {
        /* ucs2 формат */
        QByteArray a;
        for (int i = 0; i < tp_ud.length() / 4; ++i)
        {
            a.append(tp_ud.mid(i*4 + 2, 2).toInt(&ok, 16));
            a.append(tp_ud.mid(i*4, 2).toInt(&ok, 16));
        }
        smsText = QString::fromUtf16((ushort*)a.data(), a.length() / 2);
    } else if ((tp_dcs & 0x0f) == 0)
    {
        /* 7 битный формат */
        smsText = getASCIIFrom7BitCodedString(tp_ud);
    }
    _smsText = smsText;
    return true;
}


bool ModemsWorkerThread::portIsPresent(const QString &modemName)
{
    if (!modemsList.contains(modemName))
        return false;
    modemInfo *pModemInfo = &modemsList[modemName];
    SerialPortInfo si(pModemInfo->portName);
    return si.isValid();
}

bool ModemsWorkerThread::isRegisteredInNetwork(const QString &modemName)
{
    if (!modemsList.contains(modemName))
        return false;
    modemInfo *pModemInfo = &modemsList[modemName];
    return pModemInfo->isRegisteredInNetwork;
}

bool ModemsWorkerThread::portIsOpen(const QString &modemName)
{
    if (!modemsList.contains(modemName))
        return false;
    modemInfo *pModemInfo = &modemsList[modemName];
    return pModemInfo->isOpen;
}
