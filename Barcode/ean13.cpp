#include "ean13.h"

#include <QPainter>
#include <QDebug>

/** EAN13
  *
  * A class to encode and draw EAN-13 barcodes.
  *
  * Technical information and inspiration from rainman_63 whose article can be found
  * at http://www.codeproject.com/KB/graphics/ean_13_barcodes.aspx. Please give credit
  * if you use this code.
  * Wiki article at http://en.wikipedia.org/wiki/European_Article_Number
  *
  **/

EAN13::EAN13(const QString &code)
{
    // Set up constant data

    LCode << "0001101" << "0011001" << "0010011" <<"0111101" << "0100011"
            << "0110001" << "0101111" << "0111011" << "0110111" << "0001011";

    GCode << "0100111" << "0110011" << "0011011" << "0100001" << "0011101"
            << "0111001" << "0000101" << "0010001" << "0001001" << "0010111";

    RCode << "1110010" << "1100110" << "1101100" << "1000010" << "1011100"
            << "1001110" << "1010000" << "1000100" << "1001000" << "1110100";

    Parity	<< "000000" << "001011" << "001101" << "001110" << "010011"
            << "011001" << "011100" << "010101" << "010110" << "011010";

    quietZone = "000000000";
    leadTail = "101";
    separator = "01010";

    nominalWidth = 37.29;
    nominalHeight = 25.93;
    nominalFontSize = 8.0;

    barcode = code.left(12);		// no more than 12 characters
}


/** setBarcode
  *
  * Sets the barcode to the given string code
  *
  **/
void EAN13::setBarcode(const QString &code)
{
    barcode = code.left(12);		// no more than 12 characters
}

/** calculateChecksumDigit
  *
  * Calculates the checkum starting from the last character which has a weighting of
  * 3 and progressing to the beginning summing the weighted and unweighted characters.
  * The modulo 10 of the sum is subtracted from 10 and the result is reduced
  * modulo 10 to give the checksum digit
  *
  **/

void EAN13::calculateChecksumDigit()
{
    int sum = 0;
    int digit = 0;

    // calculate checksum digit

    for(int i = barcode.length() - 1; i >= 0; i--)
    {
        digit = barcode.mid(i, 1).toInt();
        if(i % 2 == 0)
        {
            sum += digit;			// odd
        }
        else
        {
            sum += digit * 3;		// even
        }
    }
    int checksum = (10 -(sum % 10)) % 10;

    checksumDigit = QString::number(checksum);
}

/** convertToDigitPattern
  *
  * Given a number, returns the pattern of 0s and 1s representing its bars according
  * to the given pattern.
  *
  **/

QString EAN13::convertToDigitPattern(QString number, QStringList &pattern)
{
    QString digitPattern("");

    for(int i = 0; i < number.length(); i++)
    {
        int index = number.mid(i, 1).toInt();
        digitPattern.append(pattern[index]);
    }

    return digitPattern;
}

/** convertLeftPattern
  *
  * converts the left side digits using the pattern dictated by the number of the
  * country code (leftmost digit). the country code is not itself included.
  *
  **/

QString EAN13::convertLeftPattern(QString left)
{
    QString string(left);

    return calculateCountryCode(string.left(1).toInt(), string.mid(1));
}

/** calculateCountryCode
  *
  * Returns the digital pattern of 0s and 1s for the string left by indexing into
  * the QStringList parity with the country code
  *
  **/

QString EAN13::calculateCountryCode(int index, QString left)
{
    QString parity = this->Parity[index];

    QString result;

    for(int i = 0; i < 6; i++)
    {
        if(parity.mid(i, 1).toInt())
            result.append(convertToDigitPattern(left.mid(i, 1), this->GCode));
        else
            result.append(convertToDigitPattern(left.mid(i, 1), this->LCode));
    }

    return result;
}

/** draw
  *
  * Draws the barcode into the given rectangle and adds the code itself underneath.
  *
  * The drawing is all relative to the given rectangle except that the font is resized
  * to maintain the correct height to width ratio based on the rectangles width.
  *
  * The width of any given bar or space is given by the number of bars in a module (7)
  * by the number of modules: 13 digits, 2 quiet zones each of 9 bars, 2 leaders each
  * of 5 bars and a separator of 3 bars for a total of 113 bars
  *
  **/

void EAN13::draw(const QRectF &rect, QPainter &painter)
{
    // calculate checksum digit

    calculateChecksumDigit();

    QString code(barcode + checksumDigit);

    // convert left hand numbers

    QString leftPattern(convertLeftPattern(code.mid(0, 7)));

    // build the EAN code

    QString ean13(quietZone + leadTail + leftPattern + separator +
                  convertToDigitPattern(code.mid(7), RCode) + leadTail + quietZone);

    painter.save();

    painter.setViewport(int(rect.x()), int(rect.y()),
                        int(rect.width()), int(rect.height()));
    painter.setWindow(/*int(rect.x()), int(rect.y()),*/0, 0,
                        int(rect.width()), int(rect.height()));
    painter.fillRect(painter.window(), Qt::white);	// set background

    qreal width = rect.width();
    qreal height = rect.height();
    qreal barWidth = width / 113;

    qreal nominalWidthInPixels = nominalWidth * painter.device()->physicalDpiX() / 25.4;
    qreal fontScale = width / nominalWidthInPixels;

    painter.setRenderHint(QPainter::Antialiasing, true);
    QFont font("arial");
    font.setPointSizeF(nominalFontSize * fontScale);
    painter.setFont(font);
    painter.setPen(Qt::SolidLine);
    painter.setPen(Qt::black);
    QBrush brush;
    brush.setStyle(Qt::SolidPattern);
    brush.setColor(Qt::black);
    painter.setBrush(brush);

    qreal textHeight = painter.boundingRect(0, 0, 0, 0, 0, ean13.mid(0,1)).height();

    painter.save();

    painter.setPen(Qt::NoPen);

    // draw the barcode lines

    for(int i = 0; i < ean13.length(); i++)
    {
        if(ean13.mid(i, 1) == "1")
        {
            if((i > 12 && i < 55) || (i > 57 && i < 101))
            {
                // draw spaces for the number

                painter.drawRect(QRectF(0, 0, barWidth, height - textHeight));
            }
            else
            {
                // draw a full line

                painter.drawRect(QRectF(0, 0, barWidth, height));
            }
        }
        painter.translate(barWidth, 0);

    }

    // draw the numbers below the line

    painter.restore();

    QString countryCode(barcode.mid(0, 1));
    QString leftCode(barcode.mid(1, 6));
    QString rightCode(barcode.mid(7, 5));

    QRectF countryCodeRect(painter.boundingRect(QRectF(0, 0, 0, 0), Qt::AlignRight,
                                                countryCode));
    QRectF leftCodeRect(painter.boundingRect(QRectF(0, 0, 0, 0), Qt::AlignHCenter,
                                             leftCode));
    QRectF rightCodeRect(painter.boundingRect(QRectF(0, 0, 0, 0), Qt::AlignHCenter,
                                              rightCode));
    QRectF checksumRect(painter.boundingRect(QRectF(0, 0, 0, 0), Qt::AlignLeft,
                                             checksumDigit));

    // draw 1st digit of country code

    painter.translate(8 * barWidth, height - countryCodeRect.height());
    painter.drawText(countryCodeRect, Qt::AlignRight, countryCode);

    // draw manufacturer number

    painter.translate((25 * barWidth), 0);
    painter.drawText(leftCodeRect, Qt::AlignHCenter, leftCode);

    // draw product id

    painter.translate((47 * barWidth), 0);
    painter.drawText(rightCodeRect, Qt::AlignHCenter, rightCode);

    // draw checksum digit

    painter.translate((26 * barWidth), 0);
    painter.drawText(checksumRect, Qt::AlignLeft, checksumDigit);

    painter.restore();
}
