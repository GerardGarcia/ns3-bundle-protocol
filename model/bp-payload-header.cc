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
#include "bp-payload-header.h"
#include "sdnv.h"
#include <stdio.h>
#include <vector>

NS_LOG_COMPONENT_DEFINE ("BpPayloadHeader");

namespace ns3 {

BpPayloadHeader::BpPayloadHeader ()
  : m_length (6),
    m_blockType (1),
    m_processingControlFlags (0),
    m_blockLength (0)
{ 
  NS_LOG_FUNCTION (this);
}

BpPayloadHeader::~BpPayloadHeader ()
{ 
  NS_LOG_FUNCTION (this);
}

TypeId 
BpPayloadHeader::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::BpPayloadHeader")
    .SetParent<Header> ()
    .AddConstructor<BpPayloadHeader> ()
  ;

  return tid;
}

TypeId 
BpPayloadHeader::GetInstanceTypeId (void) const
{ 
  NS_LOG_FUNCTION (this);
  return GetTypeId ();
}

uint32_t 
BpPayloadHeader::GetSerializedSize (void) const
{ 
  NS_LOG_FUNCTION (this);
  return m_length;
}

void 
BpPayloadHeader::Print (std::ostream &os) const
{ 
  NS_LOG_FUNCTION (this);
}

void 
BpPayloadHeader::Serialize (Buffer::Iterator start) const
{ 
  NS_LOG_FUNCTION (this);
  Buffer::Iterator i = start;
  Sdnv sdnv;
  std::vector<uint8_t> result; // store encoded results

  std::vector<uint8_t> processingControlFlags = sdnv.Encode (m_processingControlFlags);
  std::vector<uint8_t> blockLength = sdnv.Encode (m_blockLength);
  result.insert (result.end (), processingControlFlags.begin (), processingControlFlags.end ());
  result.insert (result.end (), blockLength.begin (), blockLength.end ());

  i.WriteU8 (m_blockType);

  for (uint32_t k = 0; k < result.size (); k++)
    i.WriteU8 (result.at (k));  
}

uint32_t 
BpPayloadHeader::Deserialize (Buffer::Iterator start)
{ 
  NS_LOG_FUNCTION (this);
  Buffer::Iterator i = start;
  Sdnv sdnv;

  m_blockType = i.ReadU8 ();
  m_processingControlFlags = (uint8_t) sdnv.Decode (i);
  m_blockLength = (uint32_t) sdnv.Decode (i);

  return GetSerializedSize ();
}


void 
BpPayloadHeader::SetBlockReplicate (bool value)
{ 
  NS_LOG_FUNCTION (this << " " << value);
  if (value)
    m_processingControlFlags |= BLOCK_REPLICATE;
  else
    m_processingControlFlags &= (~(BLOCK_REPLICATE));
}

void 
BpPayloadHeader::SetTxStatusReport (bool value)
{ 
  NS_LOG_FUNCTION (this << " " << value); 
  if (value)
    m_processingControlFlags |= TX_STATUS_REPORT;
  else
    m_processingControlFlags &= (~(TX_STATUS_REPORT));
}

void 
BpPayloadHeader::SetDeleteBlock (bool value)
{ 
  NS_LOG_FUNCTION (this << " " << value);
  if (value)
    m_processingControlFlags |= DELETE_BLOCK;
  else
    m_processingControlFlags &= (~(DELETE_BLOCK));
}

void 
BpPayloadHeader::SetLastBlock (bool value)
{ 
  NS_LOG_FUNCTION (this << " " << value);
  if (value)
    m_processingControlFlags |= LAST_BLOCK;
  else
    m_processingControlFlags &= (~(LAST_BLOCK));
}

void 
BpPayloadHeader::SetDiscardBlock (bool value)
{ 
  NS_LOG_FUNCTION (this << " " << value);
  if (value)
    m_processingControlFlags |= DISCARD_BLOCK;
  else
    m_processingControlFlags &= (~(DISCARD_BLOCK));
}

void 
BpPayloadHeader::SetForwardWithoutProcess (bool value)
{ 
  NS_LOG_FUNCTION (this << " " << value);
  if (value)
    m_processingControlFlags |= FORWARD_WITHOUT_PROCESS;
  else
    m_processingControlFlags &= (~(FORWARD_WITHOUT_PROCESS));
}

void 
BpPayloadHeader::SetEidReference (bool value)
{ 
  NS_LOG_FUNCTION (this << " " << value);
  if (value)
    m_processingControlFlags |= EID_REFERENCE;
  else
    m_processingControlFlags &= (~(EID_REFERENCE));
}

void 
BpPayloadHeader::SetBlockLength (uint32_t len)
{ 
  NS_LOG_FUNCTION (this << " " << len);
  m_blockLength = len;
}


bool 
BpPayloadHeader::BlockReplicate () const
{ 
  NS_LOG_FUNCTION (this);
  uint32_t flag = m_processingControlFlags & BLOCK_REPLICATE;
  if (flag)
    return true;
  else 
    return false;
}

bool 
BpPayloadHeader::TxStatusReport () const
{ 
  NS_LOG_FUNCTION (this);
  uint32_t flag = m_processingControlFlags & TX_STATUS_REPORT;
  if (flag)
    return true;
  else 
    return false;
}

bool 
BpPayloadHeader::DeleteBlock () const
{ 
  NS_LOG_FUNCTION (this);
  uint32_t flag = m_processingControlFlags & DELETE_BLOCK;
  if (flag)
    return true;
  else 
    return false;
}

bool 
BpPayloadHeader::LastBlock () const
{ 
  NS_LOG_FUNCTION (this);
  uint32_t flag = m_processingControlFlags & LAST_BLOCK;
  if (flag)
    return true;
  else 
    return false;
}

bool
BpPayloadHeader::DiscardBlock () const
{ 
  NS_LOG_FUNCTION (this);
  uint32_t flag = m_processingControlFlags & DISCARD_BLOCK;
  if (flag)
    return true;
  else 
    return false;
}

bool
BpPayloadHeader::ForwardWithoutProcess () const
{ 
  NS_LOG_FUNCTION (this);
  uint32_t flag = m_processingControlFlags & FORWARD_WITHOUT_PROCESS;
  if (flag)
    return true;
  else 
    return false;
}

bool 
BpPayloadHeader::EidReference () const
{ 
  NS_LOG_FUNCTION (this);
  uint32_t flag = m_processingControlFlags & EID_REFERENCE;
  if (flag)
    return true;
  else 
    return false;
}

uint32_t
BpPayloadHeader::GetBlockLength () const
{ 
  NS_LOG_FUNCTION (this);
  return m_blockLength;
}


} // namespace ns3
