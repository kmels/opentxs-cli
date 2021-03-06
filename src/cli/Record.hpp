// Copyright (c) 2018 The Open-Transactions developers
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#ifndef OPENTXS_CLI_SRC_CLI_RECORD_HPP
#define OPENTXS_CLI_SRC_CLI_RECORD_HPP

#include <opentxs/opentxs.hpp>

#include "opentxs.hpp"

#include <cstdint>
#include <string>

namespace opentxs::cli
{
class RecordList;

class Record
{
public:
    enum RecordType {
        Mail = 0,
        Transfer,
        Receipt,
        Instrument,
        Notice,
        ErrorState
    };

private:
    RecordList& backlink_;
    std::int32_t m_nBoxIndex{-1};
    std::string m_strThreadItemId;  // Will eventually replace Box Index.
    time64_t m_ValidFrom{0};
    time64_t m_ValidTo{0};
    const std::string m_str_msg_notary_id;   // Notary where msg was transmitted
    const std::string m_str_pmnt_notary_id;  // Notary the instrument is drawn
                                             // on
    const std::string m_str_unit_type_id;
    const std::string m_str_currency_tla;
    const std::string m_str_nym_id;
    const std::string m_str_account_id;
    std::string m_str_other_nym_id;
    std::string m_str_other_account_id;
    std::string m_str_name;
    std::string m_str_date;
    std::string m_str_amount;
    std::string m_str_type;
    std::string m_str_memo;
    // Contains payment instrument or mail message (or nothing, if not
    // applicable.)
    //
    std::string m_str_contents;
    // SPECIAL MAIL
    //
    // Let's say this Record is a Mail, but it's not via OT. (Perhaps it's
    // from Bitmessage or Freenet.) In that case it's a "special mail." In that
    // case, there are a few additional pieces of data we store here.
    // For example, the method ID (if one is available.) If you use Moneychanger
    // to send or receive a message through Bitmessage, the sender or recipient
    // will have a Method ID which references the connection string used to send
    // or receive. There will also be the message type "bitmessage" with the
    // type display string "Bitmessage", and the sender and recipient addresses.
    //
    bool m_bIsSpecialMail{false};  // Meaning a bitmessage vs an OT message.
    std::int32_t m_nMethodID{0};   // A Nym in Moneychanger might have 2
                                   // Bitmessage addresses, each used on
                                   // different BM nodes with
    // different connection strings. The Method ID is used
    // to lookup the connection string.
    std::string m_str_my_address;     // My Bitmessage address.
    std::string m_str_other_address;  // The sender or recipient's Bitmessage
                                      // address.
    std::string m_str_msg_id;    // If you want to delete a Bitmessage, you must
                                 // know the message ID.
    std::string m_str_msg_type;  // "bitmessage"
    std::string m_str_msg_type_display;  // "Bitmessage"
    // Contains transaction number of actual receipt in inbox,
    // or payment inbox, or record box. (If outpayment, contains
    // transaction number on outgoing instrument.)
    //
    TransactionNumber m_lTransactionNum{0};
    TransactionNumber m_lTransNumForDisplay{0};
    TransactionNumber m_lClosingNum{0};  // Only used for finalReceipts.

    bool m_bIsPending{false};
    bool m_bIsOutgoing{false};
    bool m_bIsRecord{false};  // record box (closed, finished, historical only.)
    bool m_bIsReceipt{false};  // It's a receipt, not a payment.
    bool m_bIsPaymentPlan{false};
    bool m_bIsSmartContract{false};
    bool m_bIsVoucher{false};
    bool m_bIsCheque{false};
    bool m_bIsInvoice{false};
    bool m_bIsCash{false};
    bool m_bIsNotice{false};
    bool m_bIsExpired{false};
    bool m_bIsCanceled{false};
    bool m_bIsFinalReceipt{false};
    bool m_bHasOriginType{false};
    originType m_originType{originType::not_applicable};
    RecordType m_RecordType{ErrorState};

    bool m_bHasSuccess{false};  // Does it even HAVE a "success" state?
                                // (Incoming cheque, for example, does NOT.)
    bool m_bIsSuccess{false};   // If it DOES have a "success" state, then
                                // is it set to a success or a failure?
    bool accept_inbox_items(
        const std::string& ACCOUNT_ID,
        std::int32_t nItemType,
        const std::string& INDICES) const;
    bool AcceptIncomingTransferOrReceipt() const;
    bool cancel_outgoing_payments(
        const std::string& NYM_ID,
        const std::string& ACCOUNT_ID,
        const std::string& INDICES) const;
    bool discard_incoming_payments(
        const std::string& TRANSPORT_NOTARY_ID,
        const std::string& NYM_ID,
        const std::string& INDICES) const;

public:
    void SetOriginType(originType theOriginType);

    // Only a few receipts (finalReceipts and paymentReceipts currently)
    // even have an origin type set. (Usually origin is already known and
    // obvious.)
    // But in a few cases, we need to know. For example if the finalReceipt
    // came from a market offer, a payment plan, or a smart contract. Which
    // is it? The origin type tells you this.
    //
    // Make SURE you check HasOriginType first, since the answer given by
    // the other functions may otherwise seem wrong.
    //
    EXPORT bool HasOriginType() const;
    EXPORT bool IsOriginTypeMarketOffer() const;
    EXPORT bool IsOriginTypePaymentPlan() const;
    EXPORT bool IsOriginTypeSmartContract() const;

    EXPORT void SetSuccess(const bool bIsSuccess);
    EXPORT bool HasSuccess(bool& bIsSuccess) const;

    EXPORT void SetClosingNum(const TransactionNumber lClosingNum);
    EXPORT bool GetClosingNum(TransactionNumber& lClosingNum) const;

    EXPORT void SetSpecialMail(bool bIsSpecial = true);
    EXPORT bool IsSpecialMail() const;
    EXPORT bool IsPending() const;
    EXPORT bool IsOutgoing() const;
    EXPORT bool IsRecord() const;
    EXPORT bool IsReceipt() const;
    EXPORT bool IsFinalReceipt() const;
    EXPORT bool IsMail() const;
    EXPORT bool IsTransfer() const;
    EXPORT bool IsCheque() const;
    EXPORT bool IsInvoice() const;
    EXPORT bool IsVoucher() const;
    EXPORT bool IsContract() const;
    EXPORT bool IsPaymentPlan() const;
    EXPORT bool IsCash() const;
    EXPORT bool IsNotice() const;
    EXPORT bool HasContents() const;
    EXPORT bool HasMemo() const;
    EXPORT bool IsExpired() const;
    EXPORT bool IsCanceled() const;
    EXPORT void SetExpired();
    EXPORT void SetCanceled();
    EXPORT void SetFinalReceipt(bool bValue = true);
    EXPORT time64_t GetValidFrom() const;
    EXPORT time64_t GetValidTo() const;
    EXPORT void SetDateRange(time64_t tValidFrom, time64_t tValidTo);
    EXPORT bool CanDeleteRecord() const;     // For completed records (not
                                             // pending.)
    EXPORT bool CanAcceptIncoming() const;   // For incoming, pending
                                             // (not-yet-accepted) instruments.
    EXPORT bool CanDiscardIncoming() const;  // For INcoming, pending
                                             // (not-yet-accepted) instruments.
    EXPORT bool CanCancelOutgoing() const;   // For OUTgoing, pending
                                             // (not-yet-accepted) instruments.
    EXPORT bool CanDiscardOutgoingCash() const;  // For OUTgoing cash. (No way
                                                 // to see if it's been
                                                 // accepted, so this lets you
                                                 // erase the record of sending
                                                 // it.)
    EXPORT bool CancelOutgoing(std::string str_via_acct) const;  // For
                                                                 // outgoing,
                                                                 // pending
    // (not-yet-accepted) instruments.
    // NOTE: str_via_acct can be blank if a
    // cheque. But if voucher, smart
    // contract, payment plan, you must
    // provide.
    EXPORT bool AcceptIncomingInstrument(
        const std::string& str_into_acct) const;  // For incoming, pending
    // (not-yet-accepted) instruments.
    EXPORT bool AcceptIncomingTransfer() const;  // For incoming, pending
                                                 // (not-yet-accepted)
                                                 // transfers.
    EXPORT bool AcceptIncomingReceipt() const;   // For incoming,
                                                 // (not-yet-accepted)
                                                 // receipts.
    EXPORT bool DiscardIncoming() const;         // For incoming, pending
                                                 // (not-yet-accepted)
                                                 // instruments.
    EXPORT bool DeleteRecord() const;  // For completed records (not pending.)
    EXPORT bool DiscardOutgoingCash() const;  // For OUTgoing cash. (No way to
                                              // see if it's been accepted, so
                                              // this lets you erase the record
                                              // of sending it.)
    EXPORT std::int32_t GetBoxIndex() const;  // If this is set to 3, for
                                              // example,
    // for a payment in the payments inbox,
    // then index 3 in that same box refers
    // to the payment corresponding to this
    // record.
    EXPORT void SetBoxIndex(std::int32_t nBoxIndex);
    EXPORT const std::string GetThreadItemId() const;
    EXPORT void SetThreadItemId(const std::string& strThreadItemId);
    EXPORT std::int32_t GetMethodID() const;  // Used by "special mail."
    EXPORT void SetMethodID(std::int32_t nMethodID);
    EXPORT const std::string& GetMsgID() const;  // Used by "special mail."
    EXPORT void SetMsgID(const std::string& str_id);
    EXPORT const std::string& GetMsgType() const;  // Used by "special mail."
    EXPORT void SetMsgType(const std::string& str_type);
    EXPORT const std::string& GetMsgTypeDisplay() const;  // Used by "special
                                                          // mail."
    EXPORT void SetMsgTypeDisplay(const std::string& str_type);
    EXPORT TransactionNumber GetTransactionNum() const;  // Trans Num of receipt
                                                         // in
                                                         // the
    // box. (Unless outpayment,
    // contains number for
    // instrument.)
    EXPORT void SetTransactionNum(TransactionNumber lTransNum);
    EXPORT TransactionNumber GetTransNumForDisplay() const;  // Trans Num of the
                                                             // cheque
    // inside the receipt in the
    // box.
    EXPORT void SetTransNumForDisplay(TransactionNumber lTransNum);
    EXPORT RecordType GetRecordType() const;
    EXPORT const std::string& GetMsgNotaryID() const;
    EXPORT const std::string& GetPmntNotaryID() const;
    EXPORT const std::string& GetUnitTypeID() const;
    EXPORT const std::string& GetCurrencyTLA() const;  // BTC, USD, etc.
    EXPORT const std::string& GetNymID() const;
    EXPORT const std::string& GetAccountID() const;
    EXPORT const std::string& GetAddress() const;     // Used by "special mail"
    EXPORT const std::string& GetOtherNymID() const;  // Could be sender OR
                                                      // recipient depending on
                                                      // whether
                                                      // incoming/outgoing.
    EXPORT const std::string& GetOtherAccountID() const;  // Could be sender OR
                                                          // recipient depending
                                                          // on whether
                                                          // incoming/outgoing.
    EXPORT const std::string& GetOtherAddress() const;    // Used by "special
                                                          // mail"
    EXPORT const std::string& GetName() const;
    EXPORT const std::string& GetDate() const;
    EXPORT const std::string& GetAmount() const;
    EXPORT const std::string& GetInstrumentType() const;
    EXPORT const std::string& GetMemo() const;
    EXPORT const std::string& GetContents() const;
    EXPORT void SetOtherNymID(const std::string& str_ID);
    EXPORT void SetOtherAccountID(const std::string& str_ID);
    EXPORT void SetAddress(const std::string& str_Address);  // Used by "special
                                                             // mail"
    EXPORT void SetOtherAddress(const std::string& str_Address);  // Used by
                                                                  // "special
                                                                  // mail"
    EXPORT void SetMemo(const std::string& str_memo);
    EXPORT void SetContents(const std::string& str_contents);
    // These don't work unless the record is for a pending
    // payment plan.
    //
    EXPORT bool HasInitialPayment() const;
    EXPORT bool HasPaymentPlan() const;

    EXPORT time64_t GetInitialPaymentDate() const;
    EXPORT time64_t GetPaymentPlanStartDate() const;
    EXPORT time64_t GetTimeBetweenPayments() const;

    EXPORT Amount GetInitialPaymentAmount() const;
    EXPORT Amount GetPaymentPlanAmount() const;

    EXPORT std::int32_t GetMaximumNoPayments() const;
    EXPORT bool FormatAmount(std::string& str_output) const;
    EXPORT bool FormatAmountWithoutSymbol(std::string& str_output);
    EXPORT bool FormatAmountLocale(
        std::string& str_output,
        const std::string& str_thousands,
        const std::string& str_decimal) const;
    EXPORT bool FormatAmountWithoutSymbolLocale(
        std::string& str_output,
        const std::string& str_thousands,
        const std::string& str_decimal);
    EXPORT bool FormatDescription(std::string& str_output) const;
    EXPORT bool FormatShortMailDescription(std::string& str_output) const;
    EXPORT bool FormatMailSubject(std::string& str_output) const;
    bool operator<(const Record& rhs);
    Record(
        RecordList& backlink,
        const std::string& str_msg_notary_id,
        const std::string& str_pmnt_notary_id,
        const std::string& str_unit_type_id,
        const std::string& str_currency_tla,
        const std::string& str_nym_id,
        const std::string& str_account_id,
        const std::string& str_name,
        const std::string& str_date,
        const std::string& str_amount,
        const std::string& str_type,
        bool bIsPending,
        bool bIsOutgoing,
        bool bIsRecord,
        bool bIsReceipt,
        RecordType eRecordType);
};
}  // namespace opentxs::cli
#endif  // OPENTXS_CLI_SRC_CLI_RECORD_HPP
