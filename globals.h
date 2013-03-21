#ifndef GLOBALS_H
#define GLOBALS_H

enum TicketStatus
{
    InWork = 0,
    Ready,
    Closed
};
enum TicketRoles
{
    TDC_Relation = Qt::UserRole + 1,
    GuaranteeId
};
enum TicketHeader
{
    TicketNumber = 0,
    TicketDate, //1
    TicketBranch, //2
    TicketClientName, //3
    TicketClientPhone, //4
    TicketDeviceName, //5
    TicketDeviceSerial, //6
    TicketProblem, //7
    TicketPrice, //8
    TicketGivenOut //9
};
enum ProductHeader
{
    ProductName = 0,
    ProductSerialNumber,
    ProductPrice,
    ProductQuantity,
    ProductGuarantee
};
enum TicketFastInfoTabs
{
    TicketJobs = 0,
    TicketComments
};

#endif // GLOBALS_H
