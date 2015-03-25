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
#ifndef BP_ROUTING_PROTOCOL_H
#define BP_ROUTING_PROTOCOL_H

#include "ns3/object.h"

namespace ns3 {

class BundleProtocol;

/**
 * \brief This is an abstract base class of bundle routing protocol
 *
 */
class BpRoutingProtocol : public Object
{
public: 
  static TypeId GetTypeId (void);

  /**
   * Constructor
   */
  BpRoutingProtocol ();

  /**
   * Destroy
   */
  virtual ~BpRoutingProtocol ();

  /**
   * Set bundle protocol
   *
   * \param bundleProtocol bundle protocol
   */
  virtual void SetBundleProtocol (Ptr<BundleProtocol> bundleProtocol) = 0;
};


}  // namespace ns3

#endif /* BP_ROUTING_PROTOCOL_H */
