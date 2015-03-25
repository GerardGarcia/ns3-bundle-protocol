/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
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
 */

// Network topology
//
//       n0 ----------- n1
//            500 Kbps
//             5 ms
//
// - Flow from n0 to n1 using bundle protocol.
// - Tracing of queues and packet receptions to file "bundle-protocol-simple.tr"
//   and pcap tracing available when tracing is turned on.

#include <string>
#include <fstream>
#include "ns3/core-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/internet-module.h"
#include "ns3/network-module.h"
#include "ns3/bp-endpoint-id.h"
#include "ns3/bundle-protocol.h"
#include "ns3/bp-static-routing-protocol.h"
#include "ns3/bundle-protocol-helper.h"
#include "ns3/bundle-protocol-container.h"

#include "ns3/bp-header.h"
#include "ns3/bp-payload-header.h"


using namespace ns3;

NS_LOG_COMPONENT_DEFINE ("BundleProtocolNoCLA");

void SendBundle (Ptr<Socket> sock, Ipv4Address dstaddr, uint16_t port);
void BindSock (Ptr<Socket> sock, Ptr<NetDevice> netdev);

int
main (int argc, char *argv[])
{

  bool tracing = true;

  ns3::PacketMetadata::Enable ();

  NS_LOG_INFO ("Create bundle nodes.");
  NodeContainer nodes;
  nodes.Create (2);

  NS_LOG_INFO ("Create channels.");
  PointToPointHelper pointToPoint;
  pointToPoint.SetDeviceAttribute ("DataRate", StringValue ("500Kbps"));
  pointToPoint.SetChannelAttribute ("Delay", StringValue ("5ms"));

  NetDeviceContainer devices;
  devices = pointToPoint.Install (nodes);

  InternetStackHelper internet;
  internet.Install (nodes);

  NS_LOG_INFO ("Assign IP Addresses.");
  Ipv4AddressHelper ipv4;
  ipv4.SetBase ("10.1.1.0", "255.255.255.0");
  Ipv4InterfaceContainer i = ipv4.Assign (devices);

  NS_LOG_INFO ("Populate routing tables.");
  Ipv4GlobalRoutingHelper::PopulateRoutingTables ();

  NS_LOG_INFO ("Prepare sockets.");
  Ptr<Socket> srcSocket = Socket::CreateSocket (nodes.Get(0), TypeId::LookupByName ("ns3::UdpSocketFactory"));
  Ptr<Socket> dstSocket = Socket::CreateSocket (nodes.Get(1), TypeId::LookupByName ("ns3::UdpSocketFactory"));

  InetSocketAddress local = InetSocketAddress (Ipv4Address::GetAny (), 9);
  srcSocket->Bind(local);
  dstSocket->Bind(local);

  Simulator::Schedule (Seconds (0.1), &SendBundle, srcSocket, i.GetAddress (1), 9);

  NS_LOG_INFO ("Prepare callbacks.");

  if (tracing)
  {
    AsciiTraceHelper ascii;
    pointToPoint.EnableAsciiAll (ascii.CreateFileStream ("bundle-protocol-simple.tr"));
    pointToPoint.EnablePcapAll ("bundle-protocol-simple", false);
  }

  NS_LOG_INFO ("Run Simulation.");
  Simulator::Stop (Seconds (1.0));
  Simulator::Run ();
  Simulator::Destroy ();
  NS_LOG_INFO ("Done.");

}

void SendBundle (Ptr<Socket> sock, Ipv4Address dstaddr, uint16_t port)
{
  // Build primary bundle header
  BpHeader bph;
  bph.SetDestinationEid (BpEndpointId("dtn://", "destNode"));
  bph.SetSourceEid (BpEndpointId("dtn://", "srcNode"));

  // Build bundle payload header
  BpPayloadHeader bpph;
  char test_payload[] = "TEST PAYLOAD!!";
  std::vector<uint8_t> payload(std::begin(test_payload), std::end(test_payload));
  bpph.SetPayload(payload);
  bpph.SetLastBlock(true);
  // Create packet
  Ptr<Packet> packet = Create<Packet> ();
  packet->AddHeader (bpph);
  packet->AddHeader (bph);

  sock->SendTo(packet, 0, InetSocketAddress(Ipv4Address(dstaddr), port));
}
