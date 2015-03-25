/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2013 University of New Brunswick
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation;
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 * Author: Dizhi Zhou <dizhi.zhou@gmail.com>
 */
#ifndef BP_HEADER_H
#define BP_HEADER_H

#include <stdint.h>
#include <string>
#include "ns3/header.h"
#include "ns3/nstime.h"
#include "ns3/buffer.h"
#include "ns3/sequence-number.h"
#include "bp-endpoint-id.h"

namespace ns3 {

/**
 * offset of endpoint id in dictionary field
 */
struct BpOffset 
{
  BpOffset ()
    : offset (0),
      length (0)
    {
    }

  uint16_t offset;  /// the string offset in dictionary field in BpHeader
  uint16_t length;  /// the string length 
};

typedef BpOffset BpOffset; /// typedef of struct BpOffset


/**
 * \brief Bundle protocol header
 *
 * The format of primary bundle header, which is defined in section 4.5 of RFC 5050.
 *
 */
class BpHeader : public Header
{
public:
  BpHeader ();
  
  virtual ~BpHeader ();

  // Setters

  // Configure bundle processing flags
  
  /**
   * \brief Bundle is/isn't a fragment
   */
  void SetIsFragment (const bool value);

  /**
   * \brief Application data unit is/isn't an administrative record
   */
  void SetIsAdmin (const bool value);

  /**
   * \brief Bundle mustn't/must be fragmented
   */
  void SetDonotFragment (const bool value);

  /**
   * \brief Custody transfer is/isn't requested
   */
  void SetCustTxReq (const bool value);

  /**
   * \brief Destination endpoint is/isn't a singleton
   */
  void SetSingletonDest (const bool value);

  /**
   * \brief Acknowledgement by application is/isn't requested
   */
  void SetAckbyAppReq (const bool value);

  /**
   * \brief Set priority field
   *
   * \param pri priority of bundle
   */
  void SetPriority (const uint8_t pri);

  /**
   * \brief Do or don't request report of bundle reception
   */
  void SetRecptionReport (const bool value);

  /**
   * \brief Do or don't request report of bundle acceptance
   */
  void SetCustAcceptReport (const bool value);

  /**
   * \brief Do or don't request report of bundle forwarding
   */
  void SetForwardReport (const bool value);

  /**
   * \brief Do or don't request report of bundle delivery
   */
  void SetDeliveryReport (const bool value);

  /**
   * \brief Do or don't request report of bundle deletion
   */
  void SetDeletionReport (const bool value);

  /**
   * \brief set the version of bundle protocol
   *
   * this is an empty method since the m_version is fixed in the 
   * BpHeader
   *
   * \param ver the version of bundle protocol
   */
  void SetVersion (uint8_t ver);

  /**
   * \brief set the length of bundle block
   *
   * \param len the lengh of bundle block
   */
  void SetBlockLength (uint32_t len);

  /**
   * \brief set the destination endpoint id and update the length
   *
   * \param dst the destination endpoint id
   */
  void SetDestinationEid (const BpEndpointId &dst);

  /**
   * \brief set the source endpoint id and update the length
   *
   * \param src the source endpoint id
   */
  void SetSourceEid (const BpEndpointId &src);

  /**
   * \brief set the report endpoint id and update the length
   *
   * \param report the report endpoint id
   */
  void SetReportEid (const BpEndpointId &report);

  /**
   * \brief set the custodian endpoint id and update the length
   *
   * \param cust the custodian endpoint id
   */
  void SetCustEid (const BpEndpointId &cust);

  /**
   * \brief set timestamp the creation timestamp time
   *
   * \param timestamp the creation timestamp time
   */
  void SetCreateTimestamp (const Time &timestamp);

  /**
   * \brief set the sequence number
   *
   * \param sequenceNumber the sequence number
   */
  void SetSequenceNumber (const SequenceNumber32 &sequenceNumber);

  /**
   * \brief set the lifetime of bundle
   *
   * \param lifetime the lifetime of bundle
   */
  void SetLifeTime (double lifetime);

  /**
   * \brief set the offset of fragment
   *
   * \param offset the offset of fragment
   */
  void SetFragOffset (uint32_t offset);

  /**
   * \brief set the total length of application data unit
   *
   * \param len the total length of application data unit
   */
  void SetAduLength (uint32_t len);


  // Getters

  // Get bundle processing flags

  /**
   * \brief Is bundle a fragment?
   */
  bool IsFragment () const;

  /**
   * \brief Is application data unit an administrative data?
   */
  bool IsAdmin () const;

  /**
   * \brief Must bundle not be fragmented?
   */
  bool DonotFragment () const;

  /**
   * \brief Is Custody transfer requested?
   */
  bool CustTxReq () const;

  /**
   * \brief Is destination endpoint a singleton?
   */
  bool SingletonDest () const;

  /**
   * \brief Is acknowledgement by application requested
   */
  bool AckbyAppReq () const; 

  /**
   * \brief Get priority of bundle
   *
   * \return priority of bundle
   */
  uint8_t Priority () const;  

  /**
   * \brief Does the bundle node need to request reporting of bundle reception?
   */
  bool RecptionReport () const;

  /**
   * \brief Does the bundle node need to request reporting of bundle acceptance?
   */
  bool CustAcceptReport () const;

  /**
   * \brief Does the bundle node need to request reporting of bundle forwarding?
   */
  bool ForwardReport () const;

  /**
   * \brief Does the bundle node need to request reporting of bundle delivery?
   */
  bool DeliveryReport () const; 

  /**
   * \brief Does the bundle node need to request reporting of bundle deletion?
   */
  bool DeletionReport () const;

  /**
   * \return the version of bundle protocol
   */
  uint8_t GetVersion () const;

  /**
   * \return the lengh of bundle block
   */
  uint32_t GetBlockLength () const;

  /**
   * \return the creation timestamp time
   */
  Time GetCreateTimestamp () const;

  /**
   * \return the creation timestamp sequence number  
   */
  SequenceNumber32 GetSequenceNumber () const;


  /**
   * \return the destination endpoint id
   */
  BpEndpointId GetDestinationEid () const;

  /**
   * \return the source endpoint id
   */
  BpEndpointId GetSourceEid () const;

  /**
   * \return the report endpoint id
   */
  BpEndpointId GetReportEid () const;

  /**
   * \return the custodian endpoint id
   */
  BpEndpointId GetCustEid () const;

  /**
   * \return the lifetime of bundle
   */
  double GetLifeTime () const;

  /**
   * \return the offset of fragment
   */
  uint32_t GetFragOffset () const;

  /**
   * \return the total length of application data unit
   */
  uint32_t GetAduLength () const;

  static TypeId GetTypeId (void);
  virtual TypeId GetInstanceTypeId (void) const;
  virtual void Print (std::ostream &os) const;
  virtual uint32_t GetSerializedSize (void) const;
  virtual void Serialize (Buffer::Iterator start) const;
  virtual uint32_t Deserialize (Buffer::Iterator start);

  /**
   * process flags
   */
  typedef enum {
    BUNDLE_IS_FRAGMENT             = 1 << 0,
    BUNDLE_IS_ADMIN                = 1 << 1,
    BUNDLE_DO_NOT_FRAGMENT         = 1 << 2,
    BUNDLE_CUSTODY_XFER_REQUESTED  = 1 << 3,
    BUNDLE_SINGLETON_DESTINATION   = 1 << 4,
    BUNDLE_ACK_BY_APP              = 1 << 5,
    BUNDLE_UNUSED                  = 1 << 6,

    BULK                           = ((0 << 8) | (0 << 7)),
    NORMAL                         = ((0 << 8) | (1 << 7)),
    EXPEDITED                      = ((1 << 8) | (0 << 7)),
    UNUSED                         = ((1 << 8) | (1 << 7)),

    REQ_REPORT_BUNDLE_RECEPTION    = 1 << 14,
    REQ_REPORT_COSTODY_ACCEPT      = 1 << 15,
    REQ_REPORT_BUNDLE_FORWARD      = 1 << 16,
    REQ_REPORT_BUNDLE_DELIVERY     = 1 << 17,
    REQ_REPORT_BUNDLE_DELETION     = 1 << 18,
    REQ_REPORT_UNUSED              = 1 << 19   
  } ProcessingFlags;  



private:
  uint16_t m_length;                      /// the length of header without string

  // primary bundle block, section 4.5.1, RFC 5050
  uint8_t m_version;                      /// the version of bundle protocol,
  uint32_t m_blockLength;                 /// block length (byte)
  uint32_t m_processingFlags;             /// bundle processing control flags, section 4.2 of RFC 5050
  BpOffset m_dstSchemeOffset;             /// scheme offset of destination endpoint id
  BpOffset m_dstSspOffset;                /// ssp offset of destination endpoint id
  BpOffset m_srcSchemeOffset;             /// scheme offset of source endpoint id
  BpOffset m_srcSspOffset;                /// ssp offset of source endpoint id
  BpOffset m_reportSchemeOffset;          /// scheme offset of report endpoint id 
  BpOffset m_reportSspOffset;             /// ssp offset of report endpoint id
  BpOffset m_custSchemeOffset;            /// scheme offset of custodian endpoint id
  BpOffset m_custSspOffset;               /// ssp offset of custodian endpoint id
  Time m_createTimestamp;                 /// creation time
  SequenceNumber32 m_timestampSeqNum;     /// sequence number
  double m_lifeTime;                      /// lifetime in seconds
  uint32_t m_dictLength;                  /// dictionary length
  std::string m_dictionary;               /// dictionary
  uint32_t m_fragOffset;                  /// fragementation offset
  uint32_t m_aduLength;                   /// application data unit length
};


} // namespace ns3

#endif /* BP_HEADER_H */
