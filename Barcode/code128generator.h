#ifndef CODE128GENERATOR_H
#define CODE128GENERATOR_H

#include <QObject>
#include <QHash>
#include <QStringList>
#include <QPainter>

class Code128Generator : public QObject
{   
    Q_OBJECT
public:

    enum Code128Type
    {
        A = 0,
        B,
        C
    };


    explicit Code128Generator(const int& width,
                              const int& height,
                              const QString& text,
                              QObject *parent = 0,
                              const Code128Type& type = B
                              );
    
    void setBarWidth(const int& width);
    void setBarcodeHeight(const int& height);
    void setText(const QString& text);    
    void drawBarcode(const QRectF &rect, QPainter &painter);

private:
    void fillWeights();
    void fillCharToWeightsHash();
    QString calculateWeights();
    QString getStartZoneWeights();
    QString getDataZoneWeights();
    QString getChecksumZoneWeights();
    int calculateChecksum();

private:
    QString m_text;
    int barWidth;
    int barcodeHeight;
    QStringList weights;    
    QString stop;
    int checkSum;    
    Code128Type m_type;
    QHash<QChar,QString> charToWeightsHash;
    QHash<int,QString> charToWeightsHashC;
    QHash<int,QString> additionalWeights;
};

#endif // CODE128GENERATOR_H
