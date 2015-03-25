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
#ifndef BP_PAYLOAD_HEADER_H
#define BP_PAYLOAD_HEADER_H

#include <stdint.h>
#include "ns3/header.h"
#include "ns3/buffer.h"

namespace ns3 {

/**
 * \brief Bundle payload block header
 *
 * The format of bundle payload block header, which is defined in section 4.5 of RFC 5050.
 *
 */
class BpPayloadHeader : public Header
{
public:
  BpPayloadHeader ();

  virtual ~BpPayloadHeader ();

  /**
   * block processing control flags, section 4.3 of RFC 5050
   */
  enum {
    BLOCK_REPLICATE            = 1 << 0,
    TX_STATUS_REPORT           = 1 << 1,
    DELETE_BLOCK               = 1 << 2,
    LAST_BLOCK                 = 1 << 3,
    DISCARD_BLOCK              = 1 << 4,
    FORWARD_WITHOUT_PROCESS    = 1 << 5,
    EID_REFERENCE              = 1 << 6
  }ProcessingControlFlags; 

  // Setters

  //Configure bundle processing control flags

  /**
   * \brief Block must or mustn't be replicated in every fragment
   */
  void SetBlockReplicate (bool value);

  /**
   * \brief Do or don't transmit status report if block can't be processed
   */
  void SetTxStatusReport (bool value);

  /**
   * \brief Do or don't delete bundle if block can't be processed
   */
  void SetDeleteBlock (bool value);

  /**
   * \brief Is or isn't last block
   */
  void SetLastBlock (bool value);

  /**
   * \brief Do or don't discard block if it can't be processed
   */
  void SetDiscardBlock (bool value);

  /**
   * \brief Block was or wasn't forwarded without being processed
   */
  void SetForwardWithoutProcess (bool value);

  /**
   * \brief Block does or doesn't contain an EID-reference field
   */
  void SetEidReference (bool value);

  /**
   * \brief set the block length in byte, which is the bundle payload size
   */
  void SetBlockLength (uint32_t len);

  // Getters

  //Get bundle processing control flags

  /**
   * \return Must block be replicated in every fragment?
   */
  bool BlockReplicate () const;

  /**
   * \return Does the bundle node need to send tranmit status report if block can't be processed?
   */
  bool TxStatusReport () const;

  /**
   * \return Does the bundle node need to delete bundle if block can't be processed?
   */
  bool DeleteBlock () const;

  /**
   * \return Is this last block?
   */
  bool LastBlock () const;

  /**
   * \return Does the bundle node need to discard block if it can't be processed?
   */
  bool DiscardBlock () const;

  /**
   * \return Was block forwarded without being processed?
   */
  bool ForwardWithoutProcess () const;

  /**
   * \return Does block contain an EID-reference field?
   */
  bool EidReference () const;

  /**
   * \return the block length in byte, which is the bundle payload size
   */
  uint32_t GetBlockLength () const;

  static TypeId GetTypeId (void);
  virtual TypeId GetInstanceTypeId (void) const;
  virtual void Print (std::ostream &os) const;
  virtual uint32_t GetSerializedSize (void) const;
  virtual void Serialize (Buffer::Iterator start) const;
  virtual uint32_t Deserialize (Buffer::Iterator start);

private:
  uint16_t m_length;                 /// the length of header
  uint8_t m_blockType;               /// block type
  uint8_t m_processingControlFlags;  /// block processing control flags
  uint32_t m_blockLength;            /// block length
};



} // namespace ns3

#endif /* BP_PAYLOAD_HEADER_H */
