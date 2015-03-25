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

#ifndef BUNDLE_PROTOCOL_HELPER_H
#define BUNDLE_PROTOCOL_HELPER_H

#include <stdint.h>
#include <string>
#include "ns3/object-factory.h"
#include "ns3/attribute.h"
#include "ns3/net-device.h"
#include "ns3/node-container.h"
#include "ns3/bundle-protocol-container.h"
#include "ns3/bundle-protocol.h"
#include "ns3/bp-routing-protocol.h"
#include "ns3/bp-endpoint-id.h"

namespace ns3 {

/**
 * \brief A helper to make it easier to instantiate an ns3::BundleProtocol
 * on a set of nodes.
 */
class BundleProtocolHelper
{
public:
  /**
   * Create an BundleProtocolHelper to make it easier to work with BundleProtocol
   */
  BundleProtocolHelper ();

  /**
   * Install an ns3::BundleProtocol on each node of the input container
   * configured with all the attributes set with SetAttribute.
   *
   * \param c NodeContainer of the set of nodes on which an BundleProtocol
   * will be installed.
   *
   * \returns Container of Ptr to the BundleProtocols installed.
   */
  BundleProtocolContainer Install (NodeContainer c);

  /**
   * Install an ns3::BundleProtocol on the node configured with all the
   * attributes set with SetAttribute.
   *
   * \param node The node on which an BundleProtocol will be installed.
   * \returns Container of Ptr to the BundleProtocols installed.
   */
  BundleProtocolContainer Install (Ptr<Node> node);

  /**
   * Install an ns3::BundleProtocol on the node configured with all the
   * attributes set with SetAttribute.
   *
   * \param nodeName The node on which an BundleProtocol will be installed.
   * \returns Container of Ptr to the BundleProtocols installed.
   */
  BundleProtocolContainer Install (std::string nodeName);

  /**
   * Set endpoint id
   *
   * \param eid endpoint id
   */
  void SetBpEndpointId (BpEndpointId eid);

  /**
   * Set bundle routing protocol
   *
   * \param rt bundle routing protocol
   */
  void SetRoutingProtocol (Ptr<BpRoutingProtocol> rt);

private:
  /**
   * \internal
   * Install an ns3::BundleProtocol on the node
   *
   * \param node The node on which an BundleProtocol will be installed.
   * \returns Ptr to the BundleProtocol installed.
   */
  Ptr<BundleProtocol> InstallPriv (Ptr<Node> node);

private:
  BpEndpointId m_eid;                        /// endpoint id
  Ptr<BpRoutingProtocol> m_routingProtocol;  /// bundle routing protocol
};

} // namespace ns3

#endif /* ON_OFF_HELPER_H */

