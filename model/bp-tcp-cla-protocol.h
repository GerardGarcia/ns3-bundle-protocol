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

#ifndef BP_TCP_CLA_PROTOCOL_H
#define BP_TCP_CLA_PROTOCOL_H

#include "ns3/ptr.h"
#include "ns3/object-factory.h"
#include "bp-cla-protocol.h"
#include "bp-endpoint-id.h"
#include "ns3/socket.h"
#include "ns3/packet.h"
#include "bundle-protocol.h"
#include "bp-routing-protocol.h"
#include <map>

namespace ns3 {

class Node;
//class Socket;
//class BpSocket;

class BpTcpClaProtocol : public BpClaProtocol
{
public:

  static TypeId GetTypeId (void);

  /**
   * \brief Constructor
   */
  BpTcpClaProtocol ();

  /**
   * Destroy
   */
  virtual ~BpTcpClaProtocol ();

  /**
   * send packet to the transport layer
   *
   * \param packet packet to sent
   */
  virtual int SendPacket (Ptr<Packet> packet);

  /**
   * Set the TCP socket in listen state;
   *
   * \param local the local endpoint id
   */
  virtual int EnableReceive (const BpEndpointId &local);

  /**
   * Close the TCP connection
   *
   * \param local the endpoint id of registration
   */
  virtual int DisableReceive (const BpEndpointId &local);


  /**
   * Enable this bundle node to send bundles at the transport layer
   * This method calls the bind (), connect primitives of tcp socket
   *
   * \param src the source endpoint id
   * \param dst the destination endpoint id
   */
  virtual int EnableSend (const BpEndpointId &src, const BpEndpointId &dst);


  /**
   * \brief Get the transport layer socket
   *
   * This method finds the socket by the source endpoint id of the bundle. 
   * If it cannot find, which means that this bundle is the first bundle required
   * to be transmitted for this source endpoint id, it start a tcp connection with
   * the destination endpoint id.
   *
   * \param packet the bundle required to be transmitted
   *
   * \return return NULL if it cannot find a socket for the source endpoing id of 
   * the bunle. Otherwise, it returns the socket.
   */
  virtual Ptr<Socket> GetL4Socket (Ptr<Packet> packet);

  /**
   * Connect to routing protocol
   *
   * \param route routing protocol
   */
  void SetRoutingProtocol (Ptr<BpRoutingProtocol> route);

  /**
   * Get routing protocol
   *
   * \return routing protocol
   */
  virtual Ptr<BpRoutingProtocol> GetRoutingProtocol ();

  /**
   * Connect to bundle protocol
   *
   * \param bundleProtocol bundle protocol
   */
  void SetBundleProtocol (Ptr<BundleProtocol> bundleProtocol);

  /**
   *  Callbacks methods callded by NotifyXXX methods in TcpSocketBase
   *
   *  Those methods will further call the callback wrap methods in BpSocket
   *  The only exception is the DataRecv (), which will call BpSocket::Receive ()
   *
   *  The operations within those methods will be updated in the furture versions
   */

  /**
   * \brief connection succeed callback
   */
  void ConnectionSucceeded (Ptr<Socket> socket );

  /**
   * \brief connection fail callback
   */
  void ConnectionFailed (Ptr<Socket> socket);

  /**
   * \brief normal close callback
   */
  void NormalClose (Ptr<Socket> socket);

  /**
   * \brief error close callback
   */
  void ErrorClose (Ptr<Socket> socket);

  /**
   * \brief connection succeed callback
   */
  bool ConnectionRequest (Ptr<Socket>,const Address &);

  /**
   * \brief new connection created callback
   */
  void NewConnectionCreated (Ptr<Socket>, const Address &);

  /**
   * \brief data sent callback
   */
  void DataSent (Ptr<Socket>,uint32_t size);

  /**
   * \brief sent callback
   */
  void Sent (Ptr<Socket>,uint32_t size);

  /**
   * \brief data receive callback
   */
  void DataRecv (Ptr<Socket> socket);

private:

  /**
   * Set callbacks of the transport layer
   *
   * \param socket the transport layer socket
   */
  virtual void SetL4SocketCallbacks (Ptr<Socket> socket);

private:
  Ptr<BundleProtocol> m_bp;                             /// bundle protocol
  std::map<BpEndpointId, Ptr<Socket> > m_l4SendSockets; /// the transport layer sender sockets
  std::map<BpEndpointId, Ptr<Socket> > m_l4RecvSockets; /// the transport layer receiver sockets

  Ptr<BpRoutingProtocol> m_bpRouting;                   /// bundle routing protocol
};

} // namespace ns3

#endif /* BP_TCP_CLA_PROTOCOL_H */

