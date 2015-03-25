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
#ifndef BUNDLE_PROTOCOL_H
#define BUNDLE_PROTOCOL_H

#include "bp-cla-protocol.h"
#include "bp-endpoint-id.h"
#include "bp-routing-protocol.h"
#include "ns3/sequence-number.h"
#include "ns3/object.h"
#include "ns3/event-id.h"
#include "ns3/nstime.h"
#include <string>
#include <map>
#include <queue>

namespace ns3 {

/**
 * \brief the bundle protocol register information of a endpoint id
 */
struct BpRegisterInfo {
  BpRegisterInfo () 
    : lifetime (0),
      state (true)
    {
    }

  double lifetime;   /// the lifetime of a bundle in seconds
  bool state;        /// the register state of registration
};

/**
 * \ingroup bundleprotocol
 *
 * \brief The implementation of bundle protocol
 *
 * BundleProtocol class contains two main components of the bundle protocol:
 * BP APIs and bundle processing. The names and functions of BP APIs are based
 * on DTN2 (http://www.dtnrg.org/docs/code/DTN2/doc/manual/arch.html).
 * The bundle processing procedures are based on section 4, RFC 5050. In addition,
 * BundleProtocol class have two persistant storages for bundles and 
 * registrations.
 */
class BundleProtocol : public Object
{
public:

  static TypeId GetTypeId (void);
 
  BundleProtocol (void);
  virtual ~BundleProtocol (void);

  // BP APIs

  /**
   * \brief  Install BP components to a bundle node
   * 
   *  This method installs the BundleProtocol and BpClaProtocol classes 
   *  into a bundle node.
   *
   * \param node the pointer of bundle node
   */
  virtual void Open (Ptr<Node> node);

  /**
   * \brief Register a local endpoint id in the bundle protocol
   *
   *  This method adds an entry in the registration storage. If the state field 
   *  in the BpRegisterInfo is “true” (active state),  which means that this 
   *  application desires to receive bundles, it triggers the convergence layer 
   *  (CLA) protocol to enable a transport layer connection to receive packets 
   *  (e.g., listen state in TCP).
   *
   * \param eid the local endpoint id of the application
   * \param info the registration information
   *
   * \return This method returns -1 if there is a duplicated registration, or 
   * the CLA fails to enable the transport layer connection to receive packets. 
   * Otherwise, it returns 0.
   */
  virtual int Register (const BpEndpointId &eid, const struct BpRegisterInfo &info);

  /**
   * Set the registration with eid to passive state 
   *
   * This method triggers the convergence layer(CLA) to disable the transport
   * layer to receive packets.
   *
   * \param eid endpoint id to be registered
   *
   * \return This method returns -1 if the endpoing id is not registered, or 
   * the CLA fails to disable the transport layer connection. Otherwise, it 
   * returns 0.
   */
  virtual int Unregister (const BpEndpointId &eid); 


  /**
   * \brief Set the registration into active state
   *
   * This method sets the state field of the registration into "true" (active
   * state) and triggers the convergence layer protocol (CLA) to enable a transport 
   * layer connection to receive packets for this registration (e.g., listen state 
   * in TCP).
   *
   * \param eid the endpoint id of application
   *
   * \return This method returns -1 if it cannot find the registration for this
   * endpoint id,  or the CLA fails to enable the transport layer connection to 
   * receive packets. Otherwise, it returns 0.
   */
  virtual int Bind (const BpEndpointId &eid);

  /**
   * \brief Build the endpoint id by scheme and ssp
   *
   * \param scheme the scheme part of the endopoint id
   *
   * \param ssp the scheme-specific part of the endpoint id
   *
   * \return the endpoint id
   */
  virtual BpEndpointId BuildBpEndpointId (const std::string &scheme, const std::string &ssp);

  /**
   * Build the endpoint id by a uri
   *
   * \param uri the user defined uri 
   *
   * \return the endpoint id
   */
  virtual BpEndpointId BuildBpEndpointId (const std::string &uri);

  /**
   * Set a endpoint id
   *
   * \param eid endpoint id
   */
  virtual void SetBpEndpointId (BpEndpointId eid);

  /**
   * Send the packet p from the source bundle node
   *
   * This methods fragments the data from application layer into several bundles and 
   * stores the bundles into persistent bundle storage. The bundles are sent in a FIFO
   * order once the transport layer connection is available to send packets.
   *
   * \param p the bundle to be sent
   * \param src source endpoint id
   * \param dst destination endpoint id
   *
   * \return returns -1 if the source endpoint id is not registered. Otherwise, it 
   * returns 0.
   */
  virtual int Send (Ptr<Packet> p, const BpEndpointId &src, const BpEndpointId &dst);

  /**
   * Remove the registration with the eid and close the transport layer connection
   *
   * \param eid endpoint id to be removed
   */
  virtual int Close (const BpEndpointId &eid);  

  /**
   *  \brief Receive bundle with dst eid
   *
   *  This methods stores the received bundles in a persistent bundle storage. The 
   *  application can use this method to get the bundles in a FIFO order.
   *
   *  \param eid destination endpoint id
   *
   *  \return the bundle payload
   */
  virtual Ptr<Packet> Receive (const BpEndpointId &eid);

  // interfaces to convergence layer (CLA)

  /**
   * Receive bundle from the convergence layer and store the bundle into persistent
   * bundle storage
   *
   * \param packet packet received from the transport layer
   */
  void ReceivePacket (Ptr<Packet> packet);

  /**
   * Get and delete a bundle from the persistant storage
   *
   * This method is called by BpClaProtocol to get the first stored bundle 
   * from the persistant bundle storage for the source endpoint id
   *
   * \param src the source endpoint id
   *
   * \return the bundle with the source endpoint id
   */
  virtual Ptr<Packet> GetBundle (const BpEndpointId &src);

  /**
   * Get node of this bundle protocol
   *
   * \return node
   */
  Ptr<Node> GetNode () const;

  /**
   * Get the endpoint id of this bundle protocol
   *
   * \return local endpoint id
   */
  BpEndpointId GetBpEndpointId () const;

  // routing operations
  
  /**
   * Set the routing protocol 
   *
   * \param route routing protocol
   */
  void SetRoutingProtocol (Ptr<BpRoutingProtocol> route);

  /**
   * Get the routing protocol
   *
   * \return routing protocol
   */
  Ptr<BpRoutingProtocol> GetRoutingProtocol ();

  /**
   * Set the register information
   *
   * \param info register information
   */
  void SetBpRegisterInfo (struct BpRegisterInfo info);

  /**
   * \brief Specify bundle protocol start time
   *
   * \param start Start time for this bundle protocol,
   *        relative to the current simulation time.
   */
  void SetStartTime (Time start);

  /**
   * \brief Specify bundle protocol stop time
   * \param stop Stop time for this bundle protocol, relative to the
   *        current simulation time.
   */
  void SetStopTime (Time stop);


protected:

  virtual void DoDispose (void);
  virtual void DoInitialize (void);

private:

  /**
   * Receive packets from CLA layer and store the packets in rx buffer
   *
   * \param bundle from CLA layer
   */
  void ProcessBundle (Ptr<Packet> bundle);

  /**
   * Retreive bundle from rx buffer
   */
  void RetreiveBundle ();

  /**
   * \brief Bundle protocol specific startup code
   *
   * The StartBundleProtocol method calls the Register () method 
   * to register the m_eid into the bundle protocol
   */
  void StartBundleProtocol ();

  /**
   * \brief Bundle protocol specific shutdown code
   *
   * The StopBundleProtocol method calls the Close () method to remove
   * registration and close the transport layer connection of this 
   * bundle protocol
   */
  void StopBundleProtocol ();

private:
  Ptr<Node>           m_node;  /// bundle node            
  Ptr<BpClaProtocol>  m_cla;   /// convergence layer adapter (CLA)

  uint32_t m_bundleSize;       /// bundle size
  std::string m_l4Type;        /// the transport layer type
  std::string m_rtType;        /// the bundle routing protocol type

  std::map<BpEndpointId, std::queue<Ptr<Packet> > > BpSendBundleStore; /// persistant storage of sent bundles: map (source endpoint id, bundle packet queue )
  std::map<BpEndpointId, std::queue<Ptr<Packet> > > BpRecvBundleStore; /// persistant storage of received bundles: map (destination endpoint id, bundle packet queue )
  std::map<BpEndpointId, BpRegisterInfo> BpRegistration; /// persistant storage of registrations: map (local endpoint id, registration information)

  Ptr<Packet> m_bpRxBufferPacket; /// a buffer for all packets received from the CLA; bundles are retreived from this buffer

  SequenceNumber32 m_seq;         /// the bundle sequence number

  BpEndpointId m_eid;             /// unique id for endpoint id
  BpRegisterInfo m_bpRegInfo;     /// register information
  Ptr<BpRoutingProtocol> m_bpRoutingProtocol; /// bundle routing protocol

  Time m_startTime;         /// The simulation time that the bundle protocol will start
  Time m_stopTime;          /// The simulation time that the bundle protocol will end
  EventId m_startEvent;     /// The event that will fire at m_startTime to start the bundle protocol
  EventId m_stopEvent;      /// The event that will fire at m_stopTime to end the bundle protocol

};

} // namespace ns3

#endif /* BUNDLE_PROTOCOL_H */
