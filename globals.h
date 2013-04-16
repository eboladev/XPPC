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
    TicketDate = 1, //1
    TicketBranch = 2, //2
    TicketClientName = 3, //3
    TicketClientPhone = 4, //4
    TicketDeviceName, //5
    TicketDeviceSerial, //6
    TicketProblem, //7
    TicketPrice = 8, //8
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

enum MainTabs
{
    TicketsTab = 0,
    ProductsTab = 1
};

enum ReportTypes
{
    TicketReport = 0,
    JobListReport = 1,
    CashReceiptReport = 2,
    PricetTagReport = 3,
    CashCheckReport = 4
};
namespace XPPC {
enum SmsStatus
{
    InProcess = 0,
    Sended = 1,
    Delivered = 2,
    InQueue = 3,
    Aborted = 4,
    NonDelivered = 5,
    NonSended = 6,
    NotFound = 7
};
enum SmsGateways
{
    MainSms = 0
};
}
#endif // GLOBALS_H
