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
#include "ns3/packet.h"
#include "ns3/assert.h"
#include "ns3/nstime.h"
#include "ns3/boolean.h"
#include "ns3/object-vector.h"

#include "ns3/packet.h"
#include "ns3/node.h"
#include "ns3/simulator.h"
#include "ns3/socket.h"
#include "ns3/socket-factory.h"

#include "bp-tcp-cla-protocol.h"
#include "bp-cla-protocol.h"
#include "bundle-protocol.h"
#include "bp-static-routing-protocol.h"
#include "bp-header.h"
#include "bp-endpoint-id.h"
#include "ns3/tcp-socket-factory.h"
#include "ns3/inet-socket-address.h"
#include "ns3/packet.h"

// default port number of dtn bundle tcp convergence layer, which is 
// defined in draft-irtf--dtnrg-tcp-clayer-0.6
#define DTN_BUNDLE_TCP_PORT 4556 

NS_LOG_COMPONENT_DEFINE ("BpTcpClaProtocol");

namespace ns3 {

NS_OBJECT_ENSURE_REGISTERED (BpTcpClaProtocol);

TypeId 
BpTcpClaProtocol::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::BpTcpClaProtocol")
    .SetParent<BpClaProtocol> ()
    .AddConstructor<BpTcpClaProtocol> ()
  ;
  return tid;
}


BpTcpClaProtocol::BpTcpClaProtocol ()
  :m_bp (0),
   m_bpRouting (0)
{ 
  NS_LOG_FUNCTION (this);
}

BpTcpClaProtocol::~BpTcpClaProtocol ()
{ 
  NS_LOG_FUNCTION (this);
}

void 
BpTcpClaProtocol::SetBundleProtocol (Ptr<BundleProtocol> bundleProtocol)
{ 
  NS_LOG_FUNCTION (this << " " << bundleProtocol);
  m_bp = bundleProtocol;
}

Ptr<Socket>
BpTcpClaProtocol::GetL4Socket (Ptr<Packet> packet)
{ 
  NS_LOG_FUNCTION (this << " " << packet);
  BpHeader bph;
  packet->PeekHeader (bph);
  BpEndpointId dst = bph.GetDestinationEid ();
  BpEndpointId src = bph.GetSourceEid ();

  std::map<BpEndpointId, Ptr<Socket> >::iterator it = m_l4SendSockets.end ();
  it = m_l4SendSockets.find (src);
  if (it == m_l4SendSockets.end ())
    {
      // enable a tcp connection from the src endpoint id to the dst endpoint id
      if (EnableSend (src, dst) < 0)
        return NULL;
    }

  // update because EnableSende () add new socket into m_l4SendSockets
  it = m_l4SendSockets.find (src);

  return ((*it).second);
}


int
BpTcpClaProtocol::SendPacket (Ptr<Packet> packet)
{ 
  NS_LOG_FUNCTION (this << " " << packet);
  Ptr<Socket> socket = GetL4Socket (packet);

  if ( socket == NULL)
    return -1;


  // retreive bundles from queue in BundleProtocol
  BpHeader bph;
  packet->PeekHeader (bph);
  BpEndpointId src = bph.GetSourceEid ();
  Ptr<Packet> pkt = m_bp->GetBundle (src);
 
  if (pkt)
    {
      socket->Send (pkt);
      return 0;
    }

  return -1;
}

int
BpTcpClaProtocol::EnableReceive (const BpEndpointId &local)
{ 
  NS_LOG_FUNCTION (this << " " << local.Uri ());
  Ptr<BpStaticRoutingProtocol> route = DynamicCast <BpStaticRoutingProtocol> (m_bpRouting);
  InetSocketAddress addr = route->GetRoute (local);

  uint16_t port;
  InetSocketAddress defaultAddr ("127.0.0.1", 0);
  if (addr == defaultAddr)
    port = DTN_BUNDLE_TCP_PORT;
  else
    port = addr.GetPort ();

  InetSocketAddress address (Ipv4Address::GetAny (), port);

  // set tcp socket in listen state
  Ptr<Socket> socket = Socket::CreateSocket (m_bp->GetNode (), TcpSocketFactory::GetTypeId ());
  if (socket->Bind (address) < 0)
    return -1;
  if (socket->Listen () < 0)
    return -1;
  if (socket->ShutdownSend () < 0)
    return -1; 

  SetL4SocketCallbacks (socket);
 
  // store the sending socket so that the convergence layer can dispatch the hundles to different tcp connections
  std::map<BpEndpointId, Ptr<Socket> >::iterator it = m_l4RecvSockets.end ();
  it = m_l4RecvSockets.find (local);
  if (it == m_l4RecvSockets.end ())
    m_l4RecvSockets.insert (std::pair<BpEndpointId, Ptr<Socket> >(local, socket));  
  else
    return -1;


  return 0;
}

int
BpTcpClaProtocol::DisableReceive (const BpEndpointId &local)
{ 
  NS_LOG_FUNCTION (this << " " << local.Uri ());
   std::map<BpEndpointId, Ptr<Socket> >::iterator it = m_l4RecvSockets.end ();
  it = m_l4RecvSockets.find (local);
  if (it == m_l4RecvSockets.end ())
    {
      return -1;
    }
  else
    {
      // close the tcp conenction
      return ((*it).second)->Close ();
    }

  return 0;
}


int 
BpTcpClaProtocol::EnableSend (const BpEndpointId &src, const BpEndpointId &dst)
{ 
  NS_LOG_FUNCTION (this << " " << src.Uri () << " " << dst.Uri ());
  if (!m_bpRouting)
    NS_FATAL_ERROR ("BpTcpClaProtocol::SendPacket (): cannot find bundle routing protocol");

  // TBD: do not use dynamicast here
  // check route for destination endpoint id
  Ptr<BpStaticRoutingProtocol> route = DynamicCast <BpStaticRoutingProtocol> (m_bpRouting);
  InetSocketAddress address = route->GetRoute (dst);

  InetSocketAddress defaultAddr ("127.0.0.1", 0);
  if (address == defaultAddr)
    {
      NS_LOG_DEBUG ("BpTcpClaProtocol::EnableSend (): cannot find route for destination endpoint id " << dst.Uri ());
      return -1;
    }

  // start a tcp connection
  Ptr<Socket> socket = Socket::CreateSocket (m_bp->GetNode (), TcpSocketFactory::GetTypeId ());
  if (socket->Bind () < 0)
    return -1;
  if (socket->Connect (address) < 0)
    return -1;
  if (socket->ShutdownRecv () < 0)
    return -1;

  SetL4SocketCallbacks (socket);

  // store the sending socket so that the convergence layer can dispatch the hundles to different tcp connections
  std::map<BpEndpointId, Ptr<Socket> >::iterator it = m_l4SendSockets.end ();
  it = m_l4SendSockets.find (src);
  if (it == m_l4SendSockets.end ())
    m_l4SendSockets.insert (std::pair<BpEndpointId, Ptr<Socket> >(src, socket));  
  else
    return -1;

  return 0;
}

void 
BpTcpClaProtocol::SetL4SocketCallbacks (Ptr<Socket> socket)
{ 
  NS_LOG_FUNCTION (this << " " << socket);
  socket->SetConnectCallback (
    MakeCallback (&BpTcpClaProtocol::ConnectionSucceeded, this),
    MakeCallback (&BpTcpClaProtocol::ConnectionFailed, this));

  socket->SetCloseCallbacks (
    MakeCallback (&BpTcpClaProtocol::NormalClose, this),
    MakeCallback (&BpTcpClaProtocol::ErrorClose, this));

  socket->SetAcceptCallback (
    MakeCallback (&BpTcpClaProtocol::ConnectionRequest, this),
    MakeCallback (&BpTcpClaProtocol::NewConnectionCreated, this));

  socket->SetDataSentCallback (
    MakeCallback (&BpTcpClaProtocol::DataSent, this));

  socket->SetSendCallback (
    MakeCallback (&BpTcpClaProtocol::Sent, this));

  socket->SetRecvCallback (
    MakeCallback (&BpTcpClaProtocol::DataRecv, this));
}

void 
BpTcpClaProtocol::ConnectionSucceeded (Ptr<Socket> socket)
{ 
  NS_LOG_FUNCTION (this << " " << socket);
} 

void 
BpTcpClaProtocol::ConnectionFailed (Ptr<Socket> socket)
{ 
  NS_LOG_FUNCTION (this << " " << socket);
}

void 
BpTcpClaProtocol::NormalClose (Ptr<Socket> socket)
{ 
  NS_LOG_FUNCTION (this << " " << socket);
}

void 
BpTcpClaProtocol::ErrorClose (Ptr<Socket> socket)
{ 
  NS_LOG_FUNCTION (this << " " << socket);
}

bool
BpTcpClaProtocol::ConnectionRequest (Ptr<Socket> socket, const Address &address)
{ 
  NS_LOG_FUNCTION (this << " " << socket << " " << address);
  return true;
}

void 
BpTcpClaProtocol::NewConnectionCreated (Ptr<Socket> socket, const Address &address)
{ 
  NS_LOG_FUNCTION (this << " " << socket << " " << address);
  SetL4SocketCallbacks (socket);  // reset the callbacks due to fork in TcpSocketBase
}

void 
BpTcpClaProtocol::DataSent (Ptr<Socket> socket, uint32_t size)
{ 
  NS_LOG_FUNCTION (this << " " << socket << " " << size);
}

void 
BpTcpClaProtocol::Sent (Ptr<Socket> socket, uint32_t size)
{ 
  NS_LOG_FUNCTION (this << " " << socket << " " << size);
}


void 
BpTcpClaProtocol::DataRecv (Ptr<Socket> socket)
{ 
  NS_LOG_FUNCTION (this << " " << socket);
  Ptr<Packet> packet;
  Address from;
  while ((packet = socket->RecvFrom (from)))
   {
     m_bp->ReceivePacket (packet);
   }
}

void
BpTcpClaProtocol::SetRoutingProtocol (Ptr<BpRoutingProtocol> route)
{ 
  NS_LOG_FUNCTION (this << " " << route);
  m_bpRouting = route;
}

Ptr<BpRoutingProtocol>
BpTcpClaProtocol::GetRoutingProtocol ()
{ 
  NS_LOG_FUNCTION (this);
  return m_bpRouting;
}

} // namespace ns3
