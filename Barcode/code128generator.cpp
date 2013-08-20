#include "code128generator.h"

#include <QChar>
#include <QDebug>

Code128Generator::Code128Generator(const int &width, const int &height,
                                   const QString &text,
                                   QObject *parent, const Code128Type &type) :
    QObject(parent),
    m_text(text),
    barWidth(width),
    barcodeHeight(height),
    m_type(type)
{
    fillCharToWeightsHash();   
}

void Code128Generator::setBarWidth(const int &width)
{
    barWidth = width;    
}

void Code128Generator::setBarcodeHeight(const int &height)
{
    barcodeHeight = height;
}

void Code128Generator::setText(const QString &text)
{    
    m_text = text;    
}


void Code128Generator::drawBarcode(const QRectF &rect, QPainter &painter)
{    
    painter.setViewport(int(rect.x()), int(rect.y()),
                        int(rect.width()), int(rect.height()));
    painter.setWindow(/*int(rect.x()), int(rect.y()),*/0, 0,
                      int(rect.width()), int(rect.height()));
    painter.fillRect(painter.window(), Qt::white);	// set background
    painter.setPen(Qt::NoPen);
    QString weights = calculateWeights();
    bool previous = false;
    qreal xPos = 0;
    for (int j = 0 ; j < weights.length() ; ++j)
    {

        if (!previous) //draw spaces
            painter.drawRect(QRectF(xPos, 0, weights.mid(j,1).toInt()*barWidth, barcodeHeight));
        xPos += weights.mid(j,1).toInt()*barWidth;
        previous = !previous;
    }
}

void Code128Generator::fillWeights()
{
    additionalWeights[95] = "114113";
    additionalWeights[96] = "114311";
    additionalWeights[97] = "411113";
    additionalWeights[98] = "411311";
    additionalWeights[99] = "113141";
    additionalWeights[100] = "114131";
    additionalWeights[101] = "311141";
    additionalWeights[102] = "411131";
    additionalWeights[103] = "211412";
    stop = "2331112";
    weights <<"212222"
           <<"222122"
          <<"222221"
         <<"121223"
        <<"121322"
       <<"131222"
      <<"122213"
     <<"122312"
    <<"132212"
    <<"221213"
    <<"221312"
    <<"231212"
    <<"112232"
    <<"122132"
    <<"122231"
    <<"113222"
    <<"123122"
    <<"123221"
    <<"223211"
    <<"221132"
    <<"221231"
    <<"213212"
    <<"223112"
    <<"312131"
    <<"311222"
    <<"321122"
    <<"321221"
    <<"312212"
    <<"322112"
    <<"322211"
    <<"212123"
    <<"212321"
    <<"232121"
    <<"111323"
    <<"131123"
    <<"131321"
    <<"112313"
    <<"132113"
    <<"132311"
    <<"211313"
    <<"231113"
    <<"231311"
    <<"112133"
    <<"112331"
    <<"132131"
    <<"113123"
    <<"113321"
    <<"133121"
    <<"313121"
    <<"211331"
    <<"231131"
    <<"213113"
    <<"213311"
    <<"213131"
    <<"311123"
    <<"311321"
    <<"331121"
    <<"312113"
    <<"312311"
    <<"332111"
    <<"314111"
    <<"221411"
    <<"431111"
    <<"111224"
    <<"111422"
    <<"121124"
    <<"121421"
    <<"141122"
    <<"141221"
    <<"112214"
    <<"112412"
    <<"122114"
    <<"122411"
    <<"142112"
    <<"142211"
    <<"241211"
    <<"221114"
    <<"413111"
    <<"241112"
    <<"134111"
    <<"111242"
    <<"121142"
    <<"121241"
    <<"114212"
    <<"124112"
    <<"124211"
    <<"411212"
    <<"421112"
    <<"421211"
    <<"212141"
    <<"214121"
    <<"412121"
    <<"111143"
    <<"111341"
    <<"131141";
}

void Code128Generator::fillCharToWeightsHash()
{
    fillWeights();
    for (int i = 32 ; i < 127; ++i)
        charToWeightsHash[QChar(i)] = weights.at(i-32);
    for (int i = 0 ; i < 94; ++i)
        charToWeightsHashC[i] = weights.at(i);
}

QString Code128Generator::calculateWeights()
{
    QString weights;
    //add quiet zone
    weights.append("09");
    //add start zone
    weights.append(getStartZoneWeights());
    //add data zone
    weights.append(getDataZoneWeights());
    //add checksum zone
    weights.append(getChecksumZoneWeights());
    //add stop zone
    weights.append(stop);
    //add quietzone
    weights.append("9");
    return weights;
}

QString Code128Generator::getStartZoneWeights()
{
    QString start;
    switch (m_type) {
        case 0: //type A
            start = "211412";
            break;
        case 1: //B
            start = "211214";
            break;
        case 2: //C
            start = "211232";
            break;
        default: //B default
            start = "211214";
            break;
        }
    return start;
}

QString Code128Generator::getDataZoneWeights()
{
    QString weight;
    if (m_type == C)
    {
        for (int i = 0 ; i < m_text.length()-1 ; i += 2)
        {
            QString nmbr = QString("%0%1").arg(m_text.at(i)).arg(m_text.at(i+1));
            qDebug() << nmbr << m_text.at(i) << m_text.at(i+1);
            bool ok = true;
            nmbr.toInt(&ok);
            if (ok)
            {
                qDebug() << "append numberC";
                weight.append(charToWeightsHashC[nmbr.toInt()]);
            }
            else
            {
                qDebug() << "append two weights";
                weight.append(charToWeightsHash[m_text.at(i)]);
                weight.append(charToWeightsHash[m_text.at(i+1)]);
            }
        }
    }
    else
        for (int i = 0 ; i < m_text.length() ; ++i)
            weight.append(charToWeightsHash[m_text.at(i)]);
    return weight;
}

QString Code128Generator::getChecksumZoneWeights()
{
    checkSum = calculateChecksum();
    return checkSum > 94 ? additionalWeights[checkSum] : charToWeightsHash[QChar(32+checkSum)];
}

int Code128Generator::calculateChecksum()
{
    int checksum = 0;
    switch (m_type) {
    case 0: //type A
        checksum += 103;
        break;
    case 1: //B
        checksum += 104;
        break;
    case 2: //C
        checksum += 105;
        break;
    default: //B default
        checksum += 104;
        break;
    }
    if (m_type == C)
    {
        for (int i = 0 ; i < m_text.length()-1 ; i += 2)
        {
            QString nmbr = QString("%0%1").arg(m_text.at(i)).arg(m_text.at(i+1));
            bool ok = true;
            nmbr.toInt(&ok);
            if (ok)
            {
                checksum += (i+1)*nmbr.toInt();
            }
            else
            {
                checksum += (i+1)*(m_text.at(i+1).unicode()-QChar(' ').unicode());
                checksum += (i+2)*(m_text.at(i+2).unicode()-QChar(' ').unicode());
            }
        }
    }
    else
        for (int i = 0 ; i < m_text.length() ; ++i)
            checksum += (i+1)*(m_text.at(i).unicode()-QChar(' ').unicode());

    return checksum % 103;
}
