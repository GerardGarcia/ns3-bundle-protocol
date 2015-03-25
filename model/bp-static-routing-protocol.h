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
#ifndef BP_STATIC_ROUTING_PROTOCOL_H
#define BP_STATIC_ROUTING_PROTOCOL_H

#include "bp-routing-protocol.h"
#include "bundle-protocol.h"
#include "ns3/inet-socket-address.h"

namespace ns3 {

/**
 * \brief This is an abstract base class of bundle routing protocol
 *
 */
class BpStaticRoutingProtocol : public BpRoutingProtocol
{
public: 
  static TypeId GetTypeId (void);

  /**
   * Constructor
   */
  BpStaticRoutingProtocol ();

  /**
   * Destroy
   */
  virtual ~BpStaticRoutingProtocol ();

  /**
   * \brief Set static bundle protocol
   *
   * \param bundleProtocol static bundle protocol
   */
  virtual void SetBundleProtocol (Ptr<BundleProtocol> bundleProtocol);

  /**
   * \brief Add a static route 
   */
  virtual int AddRoute (BpEndpointId eid, InetSocketAddress address);

  /**
   *  \return the internet socket address of matched eid; If there is no 
   *  match route, return the 127.0.0.1 with port 0
   */
  virtual InetSocketAddress GetRoute (BpEndpointId eid);

private:
  std::map <BpEndpointId, InetSocketAddress> m_routeMap; /// routing table
  Ptr<BundleProtocol> m_bp;                              /// bundle protocol
};


}  // namespace ns3

#endif /* BP_STATIC_ROUTING_PROTOCOL_H */
