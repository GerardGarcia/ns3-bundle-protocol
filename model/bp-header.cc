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


#include "ns3/log.h"
#include "ns3/node.h"
#include "bp-header.h"
#include <stdio.h>
#include <string>

#include "sdnv.h"
#include <vector>

NS_LOG_COMPONENT_DEFINE ("BpHeader");

namespace ns3 {

BpHeader::BpHeader ()
  : m_length (66),
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
    .AddConstructor<BpHeader> ()
  ;

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
  return m_length + m_dictLength;
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
  Sdnv sdnv;
  std::vector<uint8_t> result; // store encoded results

  std::vector<uint8_t> processingFlags = sdnv.Encode (m_processingFlags);
  std::vector<uint8_t> blockLength = sdnv.Encode (m_blockLength);
  std::vector<uint8_t> dstSchemeOffset = sdnv.Encode (m_dstSchemeOffset.offset);
  std::vector<uint8_t> dstSchemeLength = sdnv.Encode (m_dstSchemeOffset.length);
  std::vector<uint8_t> dstSspOffset = sdnv.Encode (m_dstSspOffset.offset);
  std::vector<uint8_t> dstSspLength = sdnv.Encode (m_dstSspOffset.length);
  std::vector<uint8_t> srcSchemeOffset = sdnv.Encode (m_srcSchemeOffset.offset);
  std::vector<uint8_t> srcSchemeLength = sdnv.Encode (m_srcSchemeOffset.length);
  std::vector<uint8_t> srcSspOffset = sdnv.Encode (m_srcSspOffset.offset);
  std::vector<uint8_t> srcSspLength = sdnv.Encode (m_srcSspOffset.length);
  std::vector<uint8_t> reportSchemeOffset = sdnv.Encode (m_reportSchemeOffset.offset);
  std::vector<uint8_t> reportSchemeLength = sdnv.Encode (m_reportSchemeOffset.length);
  std::vector<uint8_t> reportSspOffset = sdnv.Encode (m_reportSspOffset.offset);
  std::vector<uint8_t> reportSspLength = sdnv.Encode (m_reportSspOffset.length);
  std::vector<uint8_t> custSchemeOffset = sdnv.Encode (m_custSchemeOffset.offset);
  std::vector<uint8_t> custSchemeLength = sdnv.Encode (m_custSchemeOffset.length);
  std::vector<uint8_t> custSspOffset = sdnv.Encode (m_custSspOffset.offset);
  std::vector<uint8_t> custSspLength = sdnv.Encode (m_custSspOffset.length);
  std::vector<uint8_t> createTimestamp = sdnv.Encode (m_createTimestamp.GetSeconds () * 1000);
  std::vector<uint8_t> timestampseqNum = sdnv.Encode (m_timestampSeqNum.GetValue ());
  std::vector<uint8_t> lifeTime = sdnv.Encode (m_lifeTime);
  std::vector<uint8_t> dictLength = sdnv.Encode (m_dictLength);
  std::vector<uint8_t> fragOffset = sdnv.Encode (m_fragOffset);
  std::vector<uint8_t> aduLength = sdnv.Encode (m_aduLength);

  result.insert (result.end (), processingFlags.begin (), processingFlags.end ());
  result.insert (result.end (), blockLength.begin (), blockLength.end ());
  result.insert (result.end (), dstSchemeOffset.begin (), dstSchemeOffset.end ());
  result.insert (result.end (), dstSchemeLength.begin (), dstSchemeLength.end ());
  result.insert (result.end (), dstSspOffset.begin (), dstSspOffset.end ());
  result.insert (result.end (), dstSspLength.begin (), dstSspLength.end ());
  result.insert (result.end (), srcSchemeOffset.begin (), srcSchemeOffset.end ());
  result.insert (result.end (), srcSchemeLength.begin (), srcSchemeLength.end ());
  result.insert (result.end (), srcSspOffset.begin (), srcSspOffset.end ());
  result.insert (result.end (), srcSspLength.begin (), srcSspLength.end ());
  result.insert (result.end (), reportSchemeOffset.begin (), reportSchemeOffset.end ());
  result.insert (result.end (), reportSchemeLength.begin (), reportSchemeLength.end ());
  result.insert (result.end (), reportSspOffset.begin (), reportSspOffset.end ());
  result.insert (result.end (), reportSspLength.begin (), reportSspLength.end ());
  result.insert (result.end (), custSchemeOffset.begin (), custSchemeOffset.end ());
  result.insert (result.end (), custSchemeLength.begin (), custSchemeLength.end ());
  result.insert (result.end (), custSspOffset.begin (), custSspOffset.end ());
  result.insert (result.end (), custSspLength.begin (), custSspLength.end ());
  result.insert (result.end (), createTimestamp.begin (), createTimestamp.end ());
  result.insert (result.end (), timestampseqNum.begin (), timestampseqNum.end ());
  result.insert (result.end (), lifeTime.begin (), lifeTime.end ());
  result.insert (result.end (), dictLength.begin (), dictLength.end ());

  i.WriteU8 (m_version);

  uint32_t last = 0;
  for (; last < result.size (); last++)
    i.WriteU8 (result.at (last));  

  char *str = new char[m_dictLength+1];
  memcpy (str, m_dictionary.c_str (), m_dictLength); 
  for (uint32_t k = 0; k < m_dictLength; k++)
    {
      i.WriteU8 (str[k]);
    }

  result.insert (result.end (), fragOffset.begin (), fragOffset.end ());
  result.insert (result.end (), aduLength.begin (), aduLength.end ());

  for (; last < result.size (); last++)
    i.WriteU8 (result.at (last));  

  delete str;
}

uint32_t 
BpHeader::Deserialize (Buffer::Iterator start)
{ 
  NS_LOG_FUNCTION (this);
  Buffer::Iterator i = start;
  Sdnv sdnv;

  m_version = i.ReadU8 ();
  m_processingFlags = (uint8_t) sdnv.Decode (i);
  m_blockLength = (uint32_t) sdnv.Decode (i);
  m_dstSchemeOffset.offset = (uint16_t) sdnv.Decode (i);
  m_dstSchemeOffset.length = (uint16_t) sdnv.Decode (i);
  m_dstSspOffset.offset = (uint16_t) sdnv.Decode (i); 
  m_dstSspOffset.length = (uint16_t) sdnv.Decode (i); 
  m_srcSchemeOffset.offset = (uint16_t) sdnv.Decode (i);
  m_srcSchemeOffset.length = (uint16_t) sdnv.Decode (i);
  m_srcSspOffset.offset = (uint16_t) sdnv.Decode (i);
  m_srcSspOffset.length = (uint16_t) sdnv.Decode (i);
  m_reportSchemeOffset.offset = (uint16_t) sdnv.Decode (i); 
  m_reportSchemeOffset.length = (uint16_t) sdnv.Decode (i); 
  m_reportSspOffset.offset = (uint16_t) sdnv.Decode (i);
  m_reportSspOffset.length = (uint16_t) sdnv.Decode (i);
  m_custSchemeOffset.offset = (uint16_t) sdnv.Decode (i);
  m_custSchemeOffset.length = (uint16_t) sdnv.Decode (i);
  m_custSspOffset.offset = (uint16_t) sdnv.Decode (i);
  m_custSspOffset.length = (uint16_t) sdnv.Decode (i);
  m_createTimestamp = Seconds ((double) sdnv.Decode (i) / 1000.0);
  m_timestampSeqNum = (uint32_t)sdnv.Decode (i);
  m_lifeTime = (uint64_t) sdnv.Decode (i);
  m_dictLength = (uint32_t) sdnv.Decode (i);

  char *str = new char[m_dictLength + 1];
  memcpy (str, "", m_dictLength); 
  for (uint32_t k = 0; k < m_dictLength; k++)
    {
      str[k] = i.ReadU8 ();
    }
  str[m_dictLength] = '\0';
  m_dictionary = std::string (str, m_dictLength);

  m_fragOffset = (uint32_t) sdnv.Decode (i);
  m_aduLength = (uint32_t) sdnv.Decode (i);

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
  uint32_t flag = m_processingFlags & BUNDLE_IS_FRAGMENT;
  if (flag)
    return true;
  else 
    return false;
}

bool 
BpHeader::IsAdmin () const
{ 
  NS_LOG_FUNCTION (this);
  uint32_t flag = m_processingFlags & BUNDLE_IS_ADMIN;
  if (flag)
    return true;
  else 
    return false;
}

bool 
BpHeader::DonotFragment () const
{ 
  NS_LOG_FUNCTION (this);
  uint32_t flag = m_processingFlags & BUNDLE_DO_NOT_FRAGMENT;
  if (flag)
    return true;
  else 
    return false;
}

bool 
BpHeader::CustTxReq () const
{ 
  NS_LOG_FUNCTION (this);
  uint32_t flag = m_processingFlags & BUNDLE_CUSTODY_XFER_REQUESTED;
  if (flag)
    return true;
  else 
    return false;
}

bool 
BpHeader::SingletonDest () const
{ 
  NS_LOG_FUNCTION (this);
  uint32_t flag = m_processingFlags & BUNDLE_SINGLETON_DESTINATION;
  if (flag)
    return true;
  else 
    return false;
}

bool 
BpHeader::AckbyAppReq () const
{ 
  NS_LOG_FUNCTION (this);
  uint32_t flag = m_processingFlags & BUNDLE_ACK_BY_APP;
  if (flag)
    return true;
  else 
    return false;

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
  uint32_t flag = m_processingFlags & REQ_REPORT_BUNDLE_RECEPTION;
  if (flag)
    return true;
  else 
    return false;
}

bool 
BpHeader::CustAcceptReport () const
{ 
  NS_LOG_FUNCTION (this);
  uint32_t flag = m_processingFlags & REQ_REPORT_COSTODY_ACCEPT;
  if (flag)
    return true;
  else 
    return false;
}

bool 
BpHeader::ForwardReport () const
{ 
  NS_LOG_FUNCTION (this);
  uint32_t flag = m_processingFlags & REQ_REPORT_BUNDLE_FORWARD;
  if (flag)
    return true;
  else 
    return false;
}

bool 
BpHeader::DeliveryReport () const
{ 
  NS_LOG_FUNCTION (this);
  uint32_t flag = m_processingFlags & REQ_REPORT_BUNDLE_DELIVERY;
  if (flag)
    return true;
  else 
    return false;
}

bool 
BpHeader::DeletionReport () const
{ 
  NS_LOG_FUNCTION (this);
  uint32_t flag = m_processingFlags & REQ_REPORT_BUNDLE_DELETION;
  if (flag)
    return true;
  else 
    return false;
}

void 
BpHeader::SetCreateTimestamp (const Time &timestamp)
{ 
  NS_LOG_FUNCTION (this << " " << timestamp.GetSeconds ());
  m_createTimestamp = timestamp;
}

void
BpHeader::SetSequenceNumber (const SequenceNumber32 &sequenceNumber)
{ 
  NS_LOG_FUNCTION (this << " " << sequenceNumber.GetValue ());
  m_timestampSeqNum = sequenceNumber;
}


Time 
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
  std::string ssp = dst.Ssp ();

  m_dstSchemeOffset.offset = m_dictionary.size ();
  m_dstSchemeOffset.length = scheme.size ();
  m_dictionary.append (scheme); 

  m_dstSspOffset.offset = m_dictionary.size ();
  m_dstSspOffset.length = ssp.size (); 
  m_dictionary.append (ssp);

  m_dictLength = m_dictionary.size ();
}

void 
BpHeader::SetSourceEid (const BpEndpointId &src)
{ 
  NS_LOG_FUNCTION (this << " " << src.Uri ());
  std::string scheme = src.Scheme ();
  std::string ssp = src.Ssp ();

  m_srcSchemeOffset.offset = m_dictionary.size ();
  m_srcSchemeOffset.length = scheme.size ();
  m_dictionary.append (scheme); 

  m_srcSspOffset.offset = m_dictionary.size ();
  m_srcSspOffset.length = ssp.size (); 
  m_dictionary.append (ssp);

  m_dictLength = m_dictionary.size ();

  BpEndpointId eid = GetSourceEid ();

}

void 
BpHeader::SetReportEid (const BpEndpointId &report)
{ 
  NS_LOG_FUNCTION (this << " " << report.Uri ());
  std::string scheme = report.Scheme ();
  std::string ssp = report.Ssp ();

  m_reportSchemeOffset.offset = m_dictionary.size ();
  m_reportSchemeOffset.length = scheme.size ();
  m_dictionary.append (scheme); 

  m_reportSspOffset.offset = m_dictionary.size ();
  m_reportSspOffset.length = ssp.size (); 
  m_dictionary.append (ssp);

  m_dictLength = m_dictionary.size ();
}

void 
BpHeader::SetCustEid (const BpEndpointId &cust)
{ 
  NS_LOG_FUNCTION (this << " " << cust.Uri ());
  std::string scheme = cust.Scheme ();
  std::string ssp = cust.Ssp ();

  m_custSchemeOffset.offset = m_dictionary.size ();
  m_custSchemeOffset.length = scheme.size ();
  m_dictionary.append (scheme); 

  m_custSspOffset.offset = m_dictionary.size ();
  m_custSspOffset.length = ssp.size (); 
  m_dictionary.append (ssp);

  m_dictLength = m_dictionary.size ();
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

} // namespace ns3
