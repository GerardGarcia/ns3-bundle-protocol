.. include:: replace.txt

Bundle Protocol
---------------
This model implements the base specification of bundle protocol (BP). The 
implementation is based on [rfc5050]_.

Model Description
*****************
The source code for the new module lives in the directory ``src/bundle-protocol``.

The bundle protocol (BP) can be seen as a special application, which provides transport layer 
services to other applications in a delay-tolerant network. The core structure of BP in |ns3| 
is shown below:

.. _fig-bundle-protocol-structure:

.. figure:: figures/bundle-protocol-structure.*
   :align: center

   The core structure of bundle protocol implementation.

The rectangular area represents the main classes of BP in |ns3|.

* Class ``ns3::BundleProtocol`` implements several BP APIs to applications, such as 
  Register (), Open (), Bind () and so on. Applications can use these APIs to transmit 
  bundles between bundle nodes. The names and functions of these APIs are referenced from
  a Linux bundle protocol implementation -- [DTN2]_. The other functions implemented in 
  the class ``ns3::BundleProtocol`` include bundle fragmentation and aggregation, bundle 
  storage and registration storage.

* Class ``ns3::BpClaProtocol`` is a pure abstract class for the convergence layer adaptor (CLA). 
  For each transport layer protocol, a new CLA class needs to derive from BpClaProtocol.
  In the existing implementation, only class ``ns3::BpTcpClaProtocol``, for TCP connections, is 
  implemented. It uses TCP sockets in the transport layer to transmit bundles.

* Class ``ns3::BpRoutingProtocol`` is a pure abstract class that defines the APIs of bundle
  routing protocol. In the existing implementation, only a static routing protocol class 
  ``BpStaticRoutingProtocol`` is implemented, which uses a static map between local endpoint
  id and internet socket address.

In addition to the above three core classes, the |ns3| bundle protocol model also includes classes:

* Class ``ns3::BundleProtocolHelper`` implements a bundle helper to help users easily create
  a bundle node with typical configuration. Then endpoint id data structure is supported at a BpEndpointId class;

* Class ``ns3::BundleProtocolContainer`` implements a bundle protocol container, which mimics class
  ``ns3::ApplicationContainer``.

* Class ``ns3::BpEndpointId`` implements the endpoint id data structure for bundle protocol;

* Class ``ns3::BpHeader`` implements the primary bundle header.

* Class ``ns3::BpPayloadHeader`` implements the bundle payload header.

Bundle Protocol APIs
********************
The bundle protocol model implements several key APIs:

1. Open (): method ``ns3::BundleProtocol::Open ()`` installs the ``ns3::BundleProtocol`` and ``ns3::BpClaProtocol`` 
classes into a bundle node. This method is always the first method to be called;

2. Close (): method ``ns3::BundleProtocol::Close ()`` closes the transport layer connection between two bundle nodes and 
deletes the registration information;

3. Register (): method ``ns3::BundleProtocol::Register ()`` adds an entry in the persistent registration storage. 
A registration in BP is identified by an endpoint id, which can be obtained by ``ns3::BundleProtocol::BuildBpEndpointId ()``.
If the state field in the register information is "true" (active state), which means that this application 
desires to receive bundles, Register () triggers the BpClaProtocol to create a transport layer connection 
to receive packets (e.g., listen state in TCP);

4. Unregister (): method ``ns3::BundleProtocol::Unregister ()`` triggers the convergence layer(CLA) to disable the transport 
layer to receive packets;

5. Bind (): method ``ns3::BundleProtocol::Bind ()`` sets the registration state of the local endpoint id to "true". If the 
state in Register () is "true" already, Bind () does nothing in this case;

6. Send (): method ``ns3::BundleProtocol::Send`` is called by applications to send a application data unit (ADU) from 
the source endpoint id to the destination endpoint id. If the ADU size is larger than the bundle size, 
ADU is divided into several bundles while each bundle includes a primary bundle header and a bundle payload 
header. The bundle will be stored in the persistent bundle storage in BundleProtocol first. Then the BpClaProtocol
establishes the transport layer connection with peer bundle node. Once the transport layer connection is 
available, the BpClaProtocol will retrieve and send the bundle by a FIFO order from the storage;

8. Receive (): method ``ns3::BundleProtocol::Receive ()`` is called by applications to fetch bundles stored from the bundle storage 
in a FIFO order. The bundle headers are removed before forwarding bundles to the application;

9. BuildBpEndpointId (): method ``ns3::BundleProtocol::BuildBpEndpointId ()`` builds an endpoint id based on scheme and ssp strings,
or a single uri strings. In BP of |ns3|, each registration has a unique endpoint id. a BundleProtocol class
can be served for multiple registrations in a bundle node; 

10. SetBpEndpointId (): method ``ns3::BundleProtocol::BuildBpEndpointId ()`` lets users to build an endpoint id and set the endpoint 
id to the bundle protocol. 

Scope and Limitations
*********************
The existing bundle protocol model in |ns3| support following functions:

1. Unicast transmission of multiple bundles between two bundle nodes;

2. Bundle fragmentation and aggregation;

3. Static bundle routing protocol;

4. Transmitting bundles via TCP protocol at the transport layer;

5. Generating endpoint id based on a pair of scheme and ssp string, or a uri string;

6. Primary bundle header and bundle payload header in a bundle;

7. Self-Delimiting Numeric Values (SDNV) support in headers [rfc6250]_;

8. A bundle protocol helper, which can install bundle protocol to a set of nodes and set the routing protocol, 
   endpoint id of bundle protocol;

9. A bundle protocol container, which can set the start and stop time of a set of bundle protocols.

In the existing model, multiple bundle protocols can be installed in a single node. Each application must create its own bundle protocol.
Each bundle protocol only has one endpoint id.

Ongong developments
===================
1. Adding custody delivery support in bundle protocol model ( from section 5.2 to 5.15, RFC 5050). This includes the header processing 
   in the sender, relay and receiver bundle nodes;

Future developments
===================
There are two main IETF RFCs for bundle protocol: RFC 4838, which defines the DTN network architecture, and RFC 5050, which details
the bundle protocol specification. The bundle protocol model in |ns3| implements the main functions of RFC 5050.

In the future, unimplemented functions of RFC 5050 can be implemented:

1. Administrative record processing (section 5.1 and section 6.0, RFC 5050);

2. Polling (section 5.16, RFC 5050).

In addition, the convergence layer can be extended by:

1. Full TCP convergence layer support [clatcp]_ ;

2. UDP convergence layer support [claudp]_ .

Also, the following functions of RFC 4838 can be implemented in the future:

1. Congestion and flow control at the bundle layer (section 3.13, RFC 4838);

2. Bundle routing protocol (section 3.8, RFC 4838);

3. Anycast and multicast (RFC 3.4, RFC 4838).

RFC 4838 only gives the design principle for above functions. The protocol details should be specified by developers.

Usage
*****
An example script in ``src/bundle-protocol/examples`` is created to show the usage of the bundle protocol model. 
The sample code below shows an example of registering an application in a bundle node to send bundles.

::
 
  BpEndpointId eidSender ("dtn", "node0");    // build an endpoint id
  BundleProtocolHelper bpSenderHelper;        // create a bundle protocol helper
  bpSenderHelper.SetBpEndpointId (eidSender); // set endpoint id
  bpSenderHelper.SetRoutingProtocol (route);  // set bundle routing protocol
  BundleProtocolContainer bpSenders = bpSenderHelper.Install (nodes.Get (0)); // install bundle protocol at node 0
  bpSenders.Start (Seconds (1.0));            // start the bundle protocol at seconds 1.0; In this step, the bundle 
                                              // protocol will calls the Register API with default register information
  bpSenders.Stop (Seconds (5.0));             // stop the bundle protocol, Close API will be called

  /*
   * send the packet from endpoint id eidSender to endpoint id eidRecv. The src and dst will be transferred into internet socket address
   * based on bundle routing protocol.
   */
  bpSender.Get (0)->Send (packet, src, dst);      

References
**********
.. [rfc5050] K. Scott, S. Burleigh, "Bundle Protocol Specification," RFC 5050, Nov. 2007
.. [DTN2] DTN2, "http://www.dtnrg.org/wiki/Code," Dec. 2013
.. [clatcp] M. Demmer, J. Ott, S. Perreault, "Delay Tolerant Networking TCP Convergence Layer Protocol," draft-irtf-dtnrg-tcp-clayer-07, Sep. 2013
.. [claudp] H. Kruse, S. Ostermann, "UDP Convergence Layers for the DTN Bundle and LTP Protocols," draft-irtf-dtnrg-udp-clayer-00, Nov. 2008
.. [rfc6250] W. Eddy, E. Davies, "Using Self-Delimiting Numeric Values in Protocols," May 2011
