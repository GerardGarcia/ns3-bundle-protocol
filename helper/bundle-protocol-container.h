/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2008 INRIA
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
 * Author: Mathieu Lacage <mathieu.lacage@sophia.inria.fr>
 * Modified by Dizhi Zhou <dizhi.zhou@gmail.com>
 */

#ifndef BUNDLE_PROTOCOL_CONTAINER_H
#define BUNDLE_PROTOCOL_CONTAINER_H

#include <stdint.h>
#include <vector>
#include "ns3/bundle-protocol.h"

namespace ns3 {

/**
 * \brief holds a vector of ns3::BundleProtocol pointers.
 *
 * Typically ns-3 BundleProtocols are installed on nodes using an BundleProtocol
 * helper.  The helper Install method takes a NodeContainer which holds 
 * some number of Ptr<Node>.  For each of the Nodes in the NodeContainer
 * the helper will instantiate an application, install it in a node and
 * add a Ptr<BundleProtocol> to that application into a Container for use
 * by the caller.  This is that container used to hold the Ptr<BundleProtocol>
 * which are instantiated by the BundleProtocol helper.
 */
class BundleProtocolContainer
{
public:
  /**
   * Create an empty BundleProtocol.
   */
  BundleProtocolContainer ();

  /**
   * Create an BundleProtocolContainer with exactly one BundleProtocol which has
   * been previously instantiated.  The single application is specified
   * by a smart pointer.
   *
   * \param bp application The Ptr<BundleProtocol> to add to the container.
   */
  BundleProtocolContainer (Ptr<BundleProtocol> bp);

  /**
   * Create an BundleProtocolContainer with exactly one BundleProtocol which has
   * been previously instantiated and assigned a name using the Object Name
   * Service.  This BundleProtocol is then specified by its assigned name. 
   *
   * \param name The name of the BundleProtocol Object to add to the container.
   */
  BundleProtocolContainer (std::string name);

  /**
   * iterator for BundleProtocol
   */
  typedef std::vector<Ptr<BundleProtocol> >::const_iterator Iterator;

  /**
   * \brief Get an iterator which refers to the first BundleProtocol in the 
   * container.
   *
   * BundleProtocols can be retrieved from the container in two ways.  First,
   * directly by an index into the container, and second, using an iterator.
   * This method is used in the iterator method and is typically used in a 
   * for-loop to run through the BundleProtocols
   *
   * \code
   *   BundleProtocolContainer::Iterator i;
   *   for (i = container.Begin (); i != container.End (); ++i)
   *     {
   *       (*i)->method ();  // some BundleProtocol method
   *     }
   * \endcode
   *
   * \returns an iterator which refers to the first BundleProtocol in the container.
   */
  Iterator Begin (void) const;

  /**
   * \brief Get an iterator which indicates past-the-last BundleProtocol in the 
   * container.
   *
   * BundleProtocols can be retrieved from the container in two ways.  First,
   * directly by an index into the container, and second, using an iterator.
   * This method is used in the iterator method and is typically used in a 
   * for-loop to run through the BundleProtocols
   *
   * \code
   *   BundleProtocolContainer::Iterator i;
   *   for (i = container.Begin (); i != container.End (); ++i)
   *     {
   *       (*i)->method ();  // some BundleProtocol method
   *     }
   * \endcode
   *
   * \returns an iterator which indicates an ending condition for a loop.
   */
  Iterator End (void) const;

  /**
   * \brief Get the number of Ptr<BundleProtocol> stored in this container.
   *
   * BundleProtocols can be retrieved from the container in two ways.  First,
   * directly by an index into the container, and second, using an iterator.
   * This method is used in the direct method and is typically used to
   * define an ending condition in a for-loop that runs through the stored
   * BundleProtocols
   *
   * \code
   *   uint32_t nBundleProtocols = container.GetN ();
   *   for (i = container.Begin (); i < nBundleProtocols; ++i)
   *     {
   *       Ptr<BundleProtocol> p = container.Get (i)
   *       (*i)->method ();  // some BundleProtocol method
   *     }
   * \endcode
   *
   * \returns the number of Ptr<BundleProtocol> stored in this container.
   */
  uint32_t GetN (void) const;

  /**
   * \brief Get the Ptr<BundleProtocol> stored in this container at a given
   * index.
   *
   * BundleProtocols can be retrieved from the container in two ways.  First,
   * directly by an index into the container, and second, using an iterator.
   * This method is used in the direct method and is used to retrieve the
   * indexed Ptr<Appliation>.
   *
   * \code
   *   uint32_t nBundleProtocols = container.GetN ();
   *   for (uint32_t i = 0 i < nBundleProtocols; ++i)
   *     {
   *       Ptr<BundleProtocol> p = container.Get (i)
   *       i->method ();  // some BundleProtocol method
   *     }
   * \endcode
   *
   * \param i the index of the requested application pointer.
   * \returns the requested application pointer.
   */
  Ptr<BundleProtocol> Get (uint32_t i) const;

  /**
   * \brief Append the contents of another BundleProtocol to the end of
   * this container.
   *
   * \param other The BundleProtocol to append.
   */
  void Add (BundleProtocolContainer other);

  /**
   * \brief Append a single Ptr<BundleProtocol> to this container.
   *
   * \param application The Ptr<BundleProtocol> to append.
   */
  void Add (Ptr<BundleProtocol> application);

  /**
   * \brief Append to this container the single Ptr<BundleProtocol> referred to
   * via its object name service registered name.
   *
   * \param name The name of the BundleProtocol Object to add to the container.
   */
  void Add (std::string name);

  /**
   * \brief Arrange for all of the BundleProtocol in this container to Start()
   * at the Time given as a parameter.
   *
   * \param start The Time at which each of the BundleProtocol should start.
   */
  void Start (Time start);

  /**
   * \brief Arrange for all of the BundleProtocol in this container to Stop()
   * at the Time given as a parameter.
   *
   * \param stop The Time at which each of the BundleProtocol should stop.
   */
  void Stop (Time stop);

private:
  std::vector<Ptr<BundleProtocol> > m_bundleProtocols; /// vector of bundle protocols
};

} // namespace ns3

#endif /* BUNDLE_PROTOCOL_CONTAINER_H */
