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
 * Author: Gerard Garcia <ggarcia@deic.uab.cat>
 */

#include "ns3/log.h"
#include "ns3/node.h"
#include "bp-header.h"
#include <stdio.h>
#include <string>

#include "sdnv.h"
#include <vector>
#include <ctime>

#define RFC_DATE_2000 946684800

NS_LOG_COMPONENT_DEFINE ("BpHeader");

namespace ns3 {

/* Private */
uint32_t
BpHeader::AddDictionaryEntry(const std::string &entry)
{
  uint32_t offset = m_dictionary.size();
  m_dictionary.append(entry);
  m_dictionary.push_back('\0');

  // Update dictionary length
  m_dictLength = m_dictionary.size ();

  return offset;
}
/* End private */


/* Public */
BpHeader::BpHeader ()
  : m_length (0),
    m_version (0x6),
    m_blockLength (0),
    m_processingFlags (0),
    m_createTimestamp (0),
    m_timestampSeqNum (0),
    m_lifeTime (0),
    m_dictLength (0),
    m_dictionary (""),
    m_fragOffset (0),
    m_aduLength (0)
{
  NS_LOG_FUNCTION (this);

  // So all unintialized offset point to this string
  AddDictionaryEntry("dtn://none");

  // Set creation timestamp
  SetCreateTimestamp(std::time(NULL));
}

BpHeader::~BpHeader ()
{
  NS_LOG_FUNCTION (this);
}


TypeId
BpHeader::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::BpHeader")
                      .SetParent<Header> ()
                      .AddConstructor<BpHeader> ();

  return tid;
}

TypeId
BpHeader::GetInstanceTypeId (void) const
{
  NS_LOG_FUNCTION (this);
  return GetTypeId ();
}

uint32_t
BpHeader::GetSerializedSize (void) const
{
  NS_LOG_FUNCTION (this);
  SDNV sdnv;
  uint32_t size = 0;
  uint64_t headerLength = 0; // Length without Version and SDNV proc. flags

  // Calculate header body length
  headerLength += sdnv.EncodingLength(m_dstSchemeOffset.offset);
  headerLength += sdnv.EncodingLength(m_dstSspOffset.offset);
  headerLength += sdnv.EncodingLength(m_srcSchemeOffset.offset);
  headerLength += sdnv.EncodingLength(m_srcSspOffset.offset);
  headerLength += sdnv.EncodingLength(m_reportSchemeOffset.offset);
  headerLength += sdnv.EncodingLength(m_reportSspOffset.offset);
  headerLength += sdnv.EncodingLength(m_custSchemeOffset.offset);
  headerLength += sdnv.EncodingLength(m_custSspOffset.offset);
  headerLength += sdnv.EncodingLength(m_createTimestamp);
  headerLength += sdnv.EncodingLength(m_timestampSeqNum.GetValue());
  headerLength += sdnv.EncodingLength(m_lifeTime);
  headerLength += sdnv.EncodingLength(m_dictLength);
  headerLength += m_dictLength;
  if (m_processingFlags & BUNDLE_IS_FRAGMENT) {
    headerLength += sdnv.EncodingLength(m_fragOffset);
    headerLength += sdnv.EncodingLength(m_aduLength);
  }

  // Calculate total length
  size = 0;
  size += sizeof(m_version);
  size += sdnv.EncodingLength(m_processingFlags);
  size += sdnv.EncodingLength(headerLength);
  size += headerLength;

  return size;
}

void
BpHeader::Print (std::ostream &os) const
{
  NS_LOG_FUNCTION (this);
}

void
BpHeader::Serialize (Buffer::Iterator start) const
{
  NS_LOG_FUNCTION (this);
  Buffer::Iterator i = start;
  SDNV sdnv;
  uint64_t headerLength = 0; // Length without Version and SDNV proc. flags
  std::vector<uint8_t> headerBody; // Body without

  std::vector<uint8_t> dstSchemeOffset = sdnv.Encode (m_dstSchemeOffset.offset);
  headerLength += dstSchemeOffset.size();
  headerBody.insert (headerBody.end (), dstSchemeOffset.begin (), dstSchemeOffset.end ());

  std::vector<uint8_t> dstSspOffset = sdnv.Encode (m_dstSspOffset.offset);
  headerLength += dstSspOffset.size();
  headerBody.insert (headerBody.end (), dstSspOffset.begin (), dstSspOffset.end ());

  std::vector<uint8_t> srcSchemeOffset = sdnv.Encode (m_srcSchemeOffset.offset);
  headerLength += srcSchemeOffset.size();
  headerBody.insert (headerBody.end (), srcSchemeOffset.begin (), srcSchemeOffset.end ());

  std::vector<uint8_t> srcSspOffset = sdnv.Encode (m_srcSspOffset.offset);
  headerLength += srcSspOffset.size();
  headerBody.insert (headerBody.end (), srcSspOffset.begin (), srcSspOffset.end ());

  std::vector<uint8_t> reportSchemeOffset = sdnv.Encode (m_reportSchemeOffset.offset);
  headerLength += reportSchemeOffset.size();
  headerBody.insert (headerBody.end (), reportSchemeOffset.begin (), reportSchemeOffset.end ());

  std::vector<uint8_t> reportSspOffset = sdnv.Encode (m_reportSspOffset.offset);
  headerLength += reportSspOffset.size();
  headerBody.insert (headerBody.end (), reportSspOffset.begin (), reportSspOffset.end ());

  std::vector<uint8_t> custSchemeOffset = sdnv.Encode (m_custSchemeOffset.offset);
  headerLength += custSchemeOffset.size();
  headerBody.insert (headerBody.end (), custSchemeOffset.begin (), custSchemeOffset.end ());

  std::vector<uint8_t> custSspOffset = sdnv.Encode (m_custSspOffset.offset);
  headerLength += custSspOffset.size();
  headerBody.insert (headerBody.end (), custSspOffset.begin (), custSspOffset.end ());

  std::vector<uint8_t> createTimestamp = sdnv.Encode (m_createTimestamp);
  headerLength += createTimestamp.size();
  headerBody.insert (headerBody.end (), createTimestamp.begin (), createTimestamp.end ());

  std::vector<uint8_t> timestampseqNum = sdnv.Encode (m_timestampSeqNum.GetValue ());
  headerLength += timestampseqNum.size();
  headerBody.insert (headerBody.end (), timestampseqNum.begin (), timestampseqNum.end ());

  std::vector<uint8_t> lifeTime = sdnv.Encode (m_lifeTime);
  headerLength += lifeTime.size();
  headerBody.insert (headerBody.end (), lifeTime.begin (), lifeTime.end ());

  std::vector<uint8_t> dictLength = sdnv.Encode (m_dictLength);
  headerLength += dictLength.size();
  headerBody.insert (headerBody.end (), dictLength.begin (), dictLength.end ());

  headerLength += m_dictLength;
  for (uint32_t k = 0; k < m_dictLength; k++) {
    headerBody.push_back(m_dictionary.at(k));
  }

  if (m_processingFlags & BUNDLE_IS_FRAGMENT) {
    std::vector<uint8_t> fragOffset = sdnv.Encode (m_fragOffset);
    headerLength += fragOffset.size();
    headerBody.insert (headerBody.end (), fragOffset.begin (), fragOffset.end ());

    std::vector<uint8_t> aduLength = sdnv.Encode (m_aduLength);
    headerLength += aduLength.size();
    headerBody.insert (headerBody.end (), aduLength.begin (), aduLength.end ());
  }

  // Version
  i.WriteU8 (m_version);

  // Proc.flags
  std::vector<uint8_t> processingFlags = sdnv.Encode (m_processingFlags);
  for (std::vector<uint8_t>::iterator it = processingFlags.begin();
       it != processingFlags.end();
       ++it) {
    i.WriteU8(*it);
  }

  // Header length
  std::vector<uint8_t> headerLengthSDNV = sdnv.Encode(headerLength);
  for (std::vector<uint8_t>::iterator it = headerLengthSDNV.begin();
       it != headerLengthSDNV.end();
       ++it) {
    i.WriteU8(*it);
  }

  // Header body
  for (std::vector<uint8_t>::iterator it = headerBody.begin();
       it != headerBody.end();
       ++it) {
    i.WriteU8(*it);
  }
}

uint32_t
BpHeader::Deserialize (Buffer::Iterator start)
{
  NS_LOG_FUNCTION (this);
  Buffer::Iterator i = start;
  SDNV sdnv;

  m_version = i.ReadU8 ();
  m_processingFlags = (uint8_t) sdnv.Decode (i);
  m_blockLength = (uint32_t) sdnv.Decode (i);
  m_dstSchemeOffset.offset = (uint16_t) sdnv.Decode (i);
  m_dstSspOffset.offset = (uint16_t) sdnv.Decode (i);
  m_srcSchemeOffset.offset = (uint16_t) sdnv.Decode (i);
  m_srcSspOffset.offset = (uint16_t) sdnv.Decode (i);
  m_reportSchemeOffset.offset = (uint16_t) sdnv.Decode (i);
  m_reportSspOffset.offset = (uint16_t) sdnv.Decode (i);
  m_custSchemeOffset.offset = (uint16_t) sdnv.Decode (i);
  m_custSspOffset.offset = (uint16_t) sdnv.Decode (i);
  m_createTimestamp = (double) sdnv.Decode (i);
  m_timestampSeqNum = (uint32_t) sdnv.Decode (i);
  m_lifeTime = (uint64_t) sdnv.Decode (i);
  m_dictLength = (uint32_t) sdnv.Decode (i);
  char str[m_dictLength];
  for (uint32_t k = 0; k < m_dictLength; k++) {
    str[k] = i.ReadU8 ();
  }
  m_dictionary = std::string (str, m_dictLength);
  if (m_processingFlags & BUNDLE_IS_FRAGMENT) {
    m_fragOffset = (uint32_t) sdnv.Decode (i);
    m_aduLength = (uint32_t) sdnv.Decode (i);
  } else {
    m_fragOffset = 0;
    m_aduLength = 0;
  }

  return GetSerializedSize ();
}


void
BpHeader::SetIsFragment (const bool value)
{
  NS_LOG_FUNCTION (this << " " << value);
  if (value)
    m_processingFlags |= BUNDLE_IS_FRAGMENT;
  else
    m_processingFlags &= (~(BUNDLE_IS_FRAGMENT));
}

void
BpHeader::SetIsAdmin (const bool value)
{
  NS_LOG_FUNCTION (this << " " << value);
  if (value)
    m_processingFlags |= BUNDLE_IS_ADMIN;
  else
    m_processingFlags &= (~(BUNDLE_IS_ADMIN));
}

void
BpHeader::SetDonotFragment (const bool value)
{
  NS_LOG_FUNCTION (this << " " << value);
  if (value)
    m_processingFlags |= BUNDLE_DO_NOT_FRAGMENT;
  else
    m_processingFlags &= (~(BUNDLE_DO_NOT_FRAGMENT));
}

void
BpHeader::SetCustTxReq (const bool value)
{
  NS_LOG_FUNCTION (this << " " << value);
  if (value)
    m_processingFlags |= BUNDLE_CUSTODY_XFER_REQUESTED;
  else
    m_processingFlags &= (~(BUNDLE_CUSTODY_XFER_REQUESTED));
}

void
BpHeader::SetSingletonDest (const bool value)
{
  NS_LOG_FUNCTION (this << " " << value);
  if (value)
    m_processingFlags |= BUNDLE_SINGLETON_DESTINATION;
  else
    m_processingFlags &= (~(BUNDLE_SINGLETON_DESTINATION));
}

void
BpHeader::SetAckbyAppReq (const bool value)
{
  NS_LOG_FUNCTION (this << " " << value);
  if (value)
    m_processingFlags |= BUNDLE_ACK_BY_APP;
  else
    m_processingFlags &= (~(BUNDLE_ACK_BY_APP));
}

void
BpHeader::SetPriority (const uint8_t pri)
{
  NS_LOG_FUNCTION (this << " " << (uint16_t)pri);
}

void
BpHeader::SetRecptionReport (const bool value)
{
  NS_LOG_FUNCTION (this << " " << value);
  if (value)
    m_processingFlags |= REQ_REPORT_BUNDLE_RECEPTION;
  else
    m_processingFlags &= (~(REQ_REPORT_BUNDLE_RECEPTION));
}

void
BpHeader::SetCustAcceptReport (const bool value)
{
  NS_LOG_FUNCTION (this << " " << value);
  if (value)
    m_processingFlags |= REQ_REPORT_COSTODY_ACCEPT;
  else
    m_processingFlags &= (~(REQ_REPORT_COSTODY_ACCEPT));
}

void
BpHeader::SetForwardReport (const bool value)
{
  NS_LOG_FUNCTION (this << " " << value);
  if (value)
    m_processingFlags |= REQ_REPORT_BUNDLE_FORWARD;
  else
    m_processingFlags &= (~(REQ_REPORT_BUNDLE_FORWARD));
}

void
BpHeader::SetDeliveryReport (const bool value)
{
  NS_LOG_FUNCTION (this << " " << value);
  if (value)
    m_processingFlags |= REQ_REPORT_BUNDLE_DELIVERY;
  else
    m_processingFlags &= (~(REQ_REPORT_BUNDLE_DELIVERY));
}

void
BpHeader::SetDeletionReport (const bool value)
{
  NS_LOG_FUNCTION (this << " " << value);
  if (value)
    m_processingFlags |= REQ_REPORT_BUNDLE_DELETION;
  else
    m_processingFlags &= (~(REQ_REPORT_BUNDLE_DELETION));
}

bool
BpHeader::IsFragment () const
{
  NS_LOG_FUNCTION (this);
  return m_processingFlags & BUNDLE_IS_FRAGMENT;
}

bool
BpHeader::IsAdmin () const
{
  NS_LOG_FUNCTION (this);
  return m_processingFlags & BUNDLE_IS_ADMIN;
}

bool
BpHeader::DonotFragment () const
{
  NS_LOG_FUNCTION (this);
  return m_processingFlags & BUNDLE_DO_NOT_FRAGMENT;
}

bool
BpHeader::CustTxReq () const
{
  NS_LOG_FUNCTION (this);
  return m_processingFlags & BUNDLE_CUSTODY_XFER_REQUESTED;
}

bool
BpHeader::SingletonDest () const
{
  NS_LOG_FUNCTION (this);
  return m_processingFlags & BUNDLE_SINGLETON_DESTINATION;
}

bool
BpHeader::AckbyAppReq () const
{
  NS_LOG_FUNCTION (this);

  return m_processingFlags & BUNDLE_ACK_BY_APP;
}

uint8_t
BpHeader::Priority () const
{
  NS_LOG_FUNCTION (this);
  return BULK;
}

bool
BpHeader::RecptionReport () const
{
  NS_LOG_FUNCTION (this);
  return m_processingFlags & REQ_REPORT_BUNDLE_RECEPTION;
}

bool
BpHeader::CustAcceptReport () const
{
  NS_LOG_FUNCTION (this);
  return m_processingFlags & REQ_REPORT_COSTODY_ACCEPT;
}

bool
BpHeader::ForwardReport () const
{
  NS_LOG_FUNCTION (this);
  return m_processingFlags & REQ_REPORT_BUNDLE_FORWARD;
}

bool
BpHeader::DeliveryReport () const
{
  NS_LOG_FUNCTION (this);
  return m_processingFlags & REQ_REPORT_BUNDLE_DELIVERY;
}

bool
BpHeader::DeletionReport () const
{
  NS_LOG_FUNCTION (this);
  return m_processingFlags & REQ_REPORT_BUNDLE_DELETION;
}

void
BpHeader::SetCreateTimestamp (const std::time_t &timestamp)
{
  NS_LOG_FUNCTION (this << " " << timestamp);
  m_createTimestamp = timestamp - RFC_DATE_2000;
}

void
BpHeader::SetSequenceNumber (const SequenceNumber32 &sequenceNumber)
{
  NS_LOG_FUNCTION (this << " " << sequenceNumber.GetValue ());
  m_timestampSeqNum = sequenceNumber;
}


std::time_t
BpHeader::GetCreateTimestamp () const
{
  NS_LOG_FUNCTION (this);
  return m_createTimestamp;
}

SequenceNumber32
BpHeader::GetSequenceNumber () const
{
  NS_LOG_FUNCTION (this);
  return m_timestampSeqNum;
}

void
BpHeader::SetDestinationEid (const BpEndpointId &dst)
{
  NS_LOG_FUNCTION (this << " " << dst.Uri ());
  std::string scheme = dst.Scheme ();
  m_dstSchemeOffset.offset = AddDictionaryEntry(scheme);
  m_dstSchemeOffset.length = scheme.size ();

  std::string ssp = dst.Ssp();
  m_dstSspOffset.offset = AddDictionaryEntry(ssp);
  m_dstSspOffset.length = ssp.size ();
}

void
BpHeader::SetSourceEid (const BpEndpointId &src)
{
  NS_LOG_FUNCTION (this << " " << src.Uri ());
  std::string scheme = src.Scheme ();
  m_srcSchemeOffset.offset = AddDictionaryEntry(scheme);
  m_srcSchemeOffset.length = scheme.size ();

  std::string ssp = src.Ssp();
  m_srcSspOffset.offset = AddDictionaryEntry(ssp);
  m_srcSspOffset.length = ssp.size ();
}

void
BpHeader::SetReportEid (const BpEndpointId &report)
{
  NS_LOG_FUNCTION (this << " " << report.Uri ());
  std::string scheme = report.Scheme ();
  m_reportSchemeOffset.offset = AddDictionaryEntry(scheme);
  m_reportSchemeOffset.length = scheme.size ();

  std::string ssp = report.Ssp();
  m_reportSspOffset.offset = AddDictionaryEntry(ssp);
  m_reportSspOffset.length = ssp.size ();
}

void
BpHeader::SetCustEid (const BpEndpointId &cust)
{
  NS_LOG_FUNCTION (this << " " << cust.Uri ());
  std::string scheme = cust.Scheme ();
  m_custSchemeOffset.offset = AddDictionaryEntry(scheme);
  m_custSchemeOffset.length = scheme.size ();

  std::string ssp = cust.Ssp();
  m_custSspOffset.offset = AddDictionaryEntry(ssp);
  m_custSspOffset.length = ssp.size ();
}

BpEndpointId
BpHeader::GetDestinationEid () const
{
  NS_LOG_FUNCTION (this);
  std::string scheme = m_dictionary.substr (m_dstSchemeOffset.offset, m_dstSchemeOffset.length);
  std::string ssp = m_dictionary.substr (m_dstSspOffset.offset, m_dstSspOffset.length);

  BpEndpointId eid (scheme, ssp);
  return eid;
}

BpEndpointId
BpHeader::GetSourceEid () const
{
  NS_LOG_FUNCTION (this);
  std::string scheme = m_dictionary.substr (m_srcSchemeOffset.offset, m_srcSchemeOffset.length);
  std::string ssp = m_dictionary.substr (m_srcSspOffset.offset, m_srcSspOffset.length);

  BpEndpointId eid (scheme, ssp);
  return eid;
}

BpEndpointId
BpHeader::GetCustEid () const
{
  NS_LOG_FUNCTION (this);
  std::string scheme = m_dictionary.substr (m_custSchemeOffset.offset, m_custSchemeOffset.length);
  std::string ssp = m_dictionary.substr (m_custSspOffset.offset, m_custSspOffset.length);

  BpEndpointId eid (scheme, ssp);
  return eid;
}

BpEndpointId
BpHeader::GetReportEid () const
{
  NS_LOG_FUNCTION (this);
  std::string scheme = m_dictionary.substr (m_reportSchemeOffset.offset, m_reportSchemeOffset.length);
  std::string ssp = m_dictionary.substr (m_reportSspOffset.offset, m_reportSspOffset.length);

  BpEndpointId eid (scheme, ssp);
  return eid;
}


void
BpHeader::SetLifeTime (double lifetime)
{
  NS_LOG_FUNCTION (this << " " << lifetime);
  m_lifeTime = lifetime;
}

double
BpHeader::GetLifeTime () const
{
  NS_LOG_FUNCTION (this);
  return m_lifeTime;
}


void
BpHeader::SetFragOffset (uint32_t offset)
{
  NS_LOG_FUNCTION (this << " " << offset);
  m_fragOffset = offset;
}

void
BpHeader::SetAduLength (uint32_t len)
{
  NS_LOG_FUNCTION (this << " " << len);
  m_aduLength = len;
}

uint32_t
BpHeader::GetFragOffset () const
{
  NS_LOG_FUNCTION (this);
  return m_fragOffset;
}

uint32_t
BpHeader::GetAduLength () const
{
  NS_LOG_FUNCTION (this);
  return m_aduLength;
}

void
BpHeader::SetBlockLength (uint32_t len)
{
  NS_LOG_FUNCTION (this << " " << len);
  m_blockLength = len;
}

uint32_t
BpHeader::GetBlockLength () const
{
  NS_LOG_FUNCTION (this);
  return m_blockLength;
}

void
BpHeader::SetVersion (uint8_t ver)
{
  NS_LOG_FUNCTION (this);
}

uint8_t
BpHeader::GetVersion () const
{
  NS_LOG_FUNCTION (this);
  return m_version;
}
/* End public */

} // namespace ns3
