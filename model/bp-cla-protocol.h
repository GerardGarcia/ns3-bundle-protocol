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

#ifndef BP_CLA_PROTOCOL_H
#define BP_CLA_PROTOCOL_H

#include "ns3/object.h"

namespace ns3 {

class Node;
class Socket;
class Packet;
class BundleProtocol;
class BpEndpointId;
class BpRoutingProtocol;


/**
 * \brief CLA protocol abstract base class 
 *
 * This is an abstract base class for CLA protocol of BP layer 
 */
class BpClaProtocol : public Object
{
public:

  static TypeId GetTypeId (void);

  /**
   * Constructor
   */
  BpClaProtocol ();

  /**
   * Destroy
   */
  virtual ~BpClaProtocol ();

  /**
   * send packet to the transport layer
   *
   * \param packet packet to send
   */
  virtual int SendPacket (Ptr<Packet> packet) = 0;

  /**
   * Connect BundleProtocol object to CLA
   *
   * \param app BundleProtocol object
   */
  virtual void SetBundleProtocol (Ptr<BundleProtocol> app) = 0;

  /**
   * Enable the transport layer to receive packets
   *
   * \param local the endpoint id of registration
   */
  virtual int EnableReceive (const BpEndpointId &local) = 0;

  /**
   * Disable the transport layer to receive packets
   *
   * \param local the endpoint id of registration
   */
  virtual int DisableReceive (const BpEndpointId &local) = 0;

  /**
   * Enable this bundle node to send bundles at the transport layer
   *
   * \param src the source endpoint id
   * \param dst the destination endpoint id
   */
  virtual int EnableSend (const BpEndpointId &src, const BpEndpointId &dst) = 0;

  /**
   * Get the transport layer socket
   *
   * \param packet the bundle required to be transmitted
   *
   * \return the sender socket
   */
  virtual Ptr<Socket> GetL4Socket (Ptr<Packet> packet) = 0;

  /**
   * Set the bundle routing protocol
   *
   * \param route routing protocol
   */
  virtual void SetRoutingProtocol (Ptr<BpRoutingProtocol> route) = 0;

  /**
   * Get the pointer of bundle routing protocol
   */
  virtual Ptr<BpRoutingProtocol> GetRoutingProtocol () = 0; 
};

} // namespace ns3

#endif /* BP_CLA_PROTOCOL_H */

