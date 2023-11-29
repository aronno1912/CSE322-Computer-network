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
///copy of me-stat.cc
#include "ns3/applications-module.h"
#include "ns3/core-module.h"
#include "ns3/csma-module.h"
#include "ns3/internet-module.h"
#include "ns3/mobility-module.h"
#include "ns3/network-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/ssid.h"
#include "ns3/yans-wifi-helper.h"

// Default Network Topology
//
//        //  Wifi 10.1.2.0
      //                 AP                AP
        //  *    *    *    *                 *    *    *    *
        //  |    |    |    |    10.1.1.0     |    |    |    |
        // n5   n6   n7   n0 -------------- n1   n2   n3   n4 . . . .
        //                   point-to-point  
        //                                   
        //                                     WIFi 10.1.3.0

using namespace ns3;

NS_LOG_COMPONENT_DEFINE("MobileModel");

double simulationTimeInSeconds = 10.0;
uint64_t totalReceivedBytes=0;  //receiversTotalRx
uint64_t pcktReceived=0;
uint64_t pcktSent=0;
Ptr<PacketSink> sink2;     //!< Pointer to the packet sink application
uint64_t lastTotalRx = 0; //!< The value of the last total received bytes
Ptr<OutputStreamWrapper> stream;
std::ofstream throughputFile1("throughput_vs_nNodesM.dat", std::ios::app);  //for varying nodes number
std::ofstream dratioFile1("deliveryRatio_vs_nNodesM.dat", std::ios::app);
std::ofstream throughputFile2("throughput_vs_nFlowsM.dat", std::ios::app);  ////for varying flow number
std::ofstream dratioFile2("deliveryRatio_vs_nFlowsM.dat", std::ios::app);
std::ofstream throughputFile3("throughput_vs_nPacketsPerSecondM.dat", std::ios::app);  //for varying nPacketsPerSecond number
std::ofstream dratioFile3("deliveryRatio_vs_nPacketsPerSecondM.dat", std::ios::app);
std::ofstream throughputFile4("throughput_vs_speedOfNodesM.dat", std::ios::app);  //for varying speedOfNodes number
std::ofstream dratioFile4("deliveryRatio_vs_speedOfNodesM.dat", std::ios::app);
 int nNodes=20;
 int nFlows = 50;
int nPacketsPerSecond = 100;
//int speedOfNodes = 5;
int speedOfNodes=5;
int datFileTrack=0; //for .dat file append track;

void
myClaculation()
{
  Time now = Simulator::Now ();                                         /* Return the simulator's virtual time. */
  double cur = (sink2->GetTotalRx () - lastTotalRx) * (double) 8 / 1e5;     /* Convert Application RX Packets to MBits. */
  std::cout << now.GetSeconds () << "s: \t" << cur << " Mbit/s" << std::endl;
  lastTotalRx = sink2->GetTotalRx ();
  std::cout << "Sink total received : " << sink2->GetTotalRx() << std::endl;
  Simulator::Schedule (MilliSeconds (100), &myClaculation);
}


// void
// CourseChange(std::string context, Ptr<const MobilityModel> model)
// {
//     Vector position = model->GetPosition();
//     NS_LOG_UNCOND(context <<
//     " x = " << position.x << ", y = " << position.y);
// }

void CourseChange(const std::string context, Ptr<const MobilityModel> model)
{
    Vector pos = model->GetPosition();
    NS_LOG_UNCOND("CourseChange: " << context << " at " << Simulator::Now().GetSeconds()
                                    << "s, position: (" << pos.x << ", " << pos.y << ")");
}
double calculateThroughput()
{
    Time now = Simulator::Now();
    //throughput = (total received bytes * 8 bits / 1e6) / simulation time
    double tp=(totalReceivedBytes*8/1e6)/simulationTimeInSeconds;
    totalReceivedBytes=0;
    if(datFileTrack==1)
    throughputFile1 << nNodes << "\t" << tp << std::endl;
    else if(datFileTrack==2)
     throughputFile2 << nFlows << "\t" << tp << std::endl;
    else if(datFileTrack==3)
      throughputFile3 << nPacketsPerSecond << "\t" << tp << std::endl;
    else if(datFileTrack==4)
       throughputFile4 << speedOfNodes << "\t" << tp << std::endl;
    return tp;
    NS_LOG_UNCOND(now.GetSeconds() << "s: \tthroughput: " << tp << " Mbit/s");
   
    
}
double packetDeliveryRatioCalculation()
{
    Time now = Simulator::Now();
    if(pcktSent!=0)
    {

        double result=(double)pcktReceived/(double)pcktSent;
    if(datFileTrack==1)
     dratioFile1 << nNodes << "\t" << result << std::endl;
     else if(datFileTrack==2)
     dratioFile2 << nFlows << "\t" << result << std::endl;
     else if(datFileTrack==3)
     dratioFile3 << nPacketsPerSecond << "\t" << result << std::endl;
     else if(datFileTrack==4)
     dratioFile4 << speedOfNodes << "\t" << result << std::endl;

        return result;
     
    }
    return 0;
    
}
// is triggered every time a packet is transmitted (sent) from a sender node. 
void transmissionTracer(Ptr< const Packet > packet)
{
    pcktSent=pcktSent+1;
}

// is triggered every time a packet is received by a receiver node. 
void rxTracer(Ptr< const Packet > packet, const Address &address)
{
    totalReceivedBytes+= packet->GetSize();
    pcktReceived=pcktReceived+1;
}


int
main(int argc, char* argv[])
{
    bool verbose = false;
    int nReceiver = 10; //ethernet er jonno (p2p ta baad e baki 3ta node k dhore)
    int nSender = 10;
   // bool tracing = false;
    //int tx_range=5; // // given fixed in spec
    //int simulationTimeInSeconds = 25;
    //int cleanupTime = 2;
   

    CommandLine cmd(__FILE__);
    cmd.AddValue("nNodes", "Number of Nodes", nNodes);
    //cmd.AddValue("verbose", "Tell echo applications to log if true", verbose);
   // cmd.AddValue("tracing", "Enable pcap tracing", tracing);
    cmd.AddValue ("nFlows","Number of total flows", nFlows);
    cmd.AddValue ("nPacketsPerSecond","Number of packets per second", nPacketsPerSecond);
    cmd.AddValue ("speedOfNodes","Speed of nodes", speedOfNodes);
     cmd.AddValue ("datFileTrack","Which one of the parameter is varying", datFileTrack);
    cmd.Parse(argc, argv);

    nSender=nNodes/2;
    nReceiver=nNodes/2;
     double rate=(nPacketsPerSecond*1024*8)/1e6;
    std::string datarate = std::to_string(rate);
    datarate+="Mbps";

    if (verbose)
    {
        LogComponentEnable("OnOffApplication", LOG_LEVEL_INFO);
        LogComponentEnable("PacketSink", LOG_LEVEL_INFO);
    }
    
    //================== wired access points=================================
    NodeContainer p2pNodes;
    p2pNodes.Create(2);
    PointToPointHelper pointToPoint;
    pointToPoint.SetDeviceAttribute("DataRate", StringValue("2Mbps"));
    pointToPoint.SetChannelAttribute("Delay", StringValue("2ms"));
    NetDeviceContainer p2pDevices;
    p2pDevices = pointToPoint.Install(p2pNodes);


   //===================Receiver Side==============================//
    NodeContainer wifiStaNodesRcv;
    wifiStaNodesRcv.Create(nReceiver);
    NodeContainer wifiApNodeRcv= p2pNodes.Get(1);
    YansWifiChannelHelper channelrcv = YansWifiChannelHelper::Default();
    channelrcv.AddPropagationLoss("ns3::RangePropagationLossModel");
    YansWifiPhyHelper phyrcv;
    phyrcv.SetChannel(channelrcv.Create()); 
    WifiMacHelper macRcv;
    Ssid ssidrcv = Ssid("ns-3-ssidRcv");  
    WifiHelper wifiRcv;
    //wifiRcv.SetRemoteStationManager ("ns3::AarfWifiManager");
    NetDeviceContainer staDevicesRcv;
    macRcv.SetType("ns3::StaWifiMac", "Ssid", SsidValue(ssidrcv), "ActiveProbing", BooleanValue(false));
    staDevicesRcv= wifiRcv.Install(phyrcv, macRcv, wifiStaNodesRcv);
    NetDeviceContainer apDevicesRcv;
    macRcv.SetType("ns3::ApWifiMac", "Ssid", SsidValue(ssidrcv));
    apDevicesRcv = wifiRcv.Install(phyrcv, macRcv, wifiApNodeRcv);
    //std::cout << "NodeCount: " << ns3::NodeList::GetNNodes() << std::endl;



   //==================Sender Side===========================//
    NodeContainer wifiStaNodes;
    wifiStaNodes.Create(nSender);
    NodeContainer wifiApNode = p2pNodes.Get(0);
    //YansWifiChannelHelper channel = YansWifiChannelHelper::Default();
    channelrcv.AddPropagationLoss("ns3::RangePropagationLossModel");
    YansWifiPhyHelper phy;
    phy.SetChannel(channelrcv.Create()); // share the same wireless medium 
    WifiMacHelper mac;
    WifiHelper wifi;
   // wifi.SetRemoteStationManager ("ns3::AarfWifiManager");
    NetDeviceContainer staDevices;
    Ssid ssid = Ssid("ns-3-ssidSend"); 
    mac.SetType("ns3::StaWifiMac", "Ssid", SsidValue(ssid), "ActiveProbing", BooleanValue(false));
    staDevices = wifi.Install(phy, mac, wifiStaNodes);
    NetDeviceContainer apDevices;
    mac.SetType("ns3::ApWifiMac", "Ssid", SsidValue(ssid));
    apDevices = wifi.Install(phy, mac, wifiApNode);
     //std::cout << "NodeCount: " << ns3::NodeList::GetNNodes() << std::endl;
    
//     MobilityHelper mobility;

// // Left side (Senders)
// mobility.SetPositionAllocator("ns3::GridPositionAllocator",
//                               "MinX", DoubleValue(-50.0),
//                               "MinY", DoubleValue(-50.0),
//                               "DeltaX", DoubleValue(10.0),
//                               "DeltaY", DoubleValue(10.0),
//                               "GridWidth", UintegerValue(3),
//                               "LayoutType", StringValue("RowFirst"));
// mobility.SetMobilityModel("ns3::ConstantVelocityMobilityModel");
// mobility.Install(wifiStaNodes);

// // Middle (AP Nodes, stationary)
// mobility.SetPositionAllocator("ns3::GridPositionAllocator",
//                               "MinX", DoubleValue(-10.0),
//                               "MinY", DoubleValue(-10.0),
//                               "DeltaX", DoubleValue(20.0),
//                               "DeltaY", DoubleValue(20.0),
//                               "GridWidth", UintegerValue(2),
//                               "LayoutType", StringValue("RowFirst"));
// mobility.SetMobilityModel("ns3::ConstantPositionMobilityModel");
// mobility.Install(p2pNodes);

// // Right side (Receivers)
// mobility.SetPositionAllocator("ns3::GridPositionAllocator",
//                               "MinX", DoubleValue(50.0),
//                               "MinY", DoubleValue(-50.0),
//                               "DeltaX", DoubleValue(10.0),
//                               "DeltaY", DoubleValue(10.0),
//                               "GridWidth", UintegerValue(3),
//                               "LayoutType", StringValue("RowFirst"));
// mobility.SetMobilityModel("ns3::ConstantVelocityMobilityModel");
// mobility.Install(wifiStaNodesRcv);

//    MobilityHelper mobility;

// // Left side (Senders)
// mobility.SetPositionAllocator("ns3::GridPositionAllocator",
//                               "MinX", DoubleValue(-11.0),
//                               "MinY", DoubleValue(0.0),
//                               "DeltaX", DoubleValue(1.0),
//                               "DeltaY", DoubleValue(1.0),
//                               "GridWidth", UintegerValue(1),
//                               "LayoutType", StringValue("RowFirst"));
// mobility.SetMobilityModel("ns3::ConstantVelocityMobilityModel");
// mobility.Install(wifiStaNodes);

// // Middle (AP Nodes, stationary)
// mobility.SetPositionAllocator("ns3::GridPositionAllocator",
//                               "MinX", DoubleValue(-10.0),
//                               "MinY", DoubleValue(nSender/2),
//                               "DeltaX", DoubleValue(1.0),
//                               "DeltaY", DoubleValue(1.0),
//                               "GridWidth", UintegerValue(1),
//                               "LayoutType", StringValue("RowFirst"));
// mobility.SetMobilityModel("ns3::ConstantPositionMobilityModel");
// mobility.Install(wifiApNode);

// mobility.SetPositionAllocator("ns3::GridPositionAllocator",
//                               "MinX", DoubleValue(10.0),
//                               "MinY", DoubleValue(nSender/2),
//                               "DeltaX", DoubleValue(1.0),
//                               "DeltaY", DoubleValue(1.0),
//                               "GridWidth", UintegerValue(1),
//                               "LayoutType", StringValue("RowFirst"));
// mobility.SetMobilityModel("ns3::ConstantPositionMobilityModel");
// mobility.Install(wifiApNodeRcv);

// // Right side (Receivers)
// mobility.SetPositionAllocator("ns3::GridPositionAllocator",
//                               "MinX", DoubleValue(11.0),
//                               "MinY", DoubleValue(0.0),
//                               "DeltaX", DoubleValue(1.0),
//                               "DeltaY", DoubleValue(1.0),
//                               "GridWidth", UintegerValue(1),
//                               "LayoutType", StringValue("RowFirst"));
// mobility.SetMobilityModel("ns3::ConstantVelocityMobilityModel");
// mobility.Install(wifiStaNodesRcv);


// // Set velocities and initial positions for Senders and Receivers
// Ptr<ConstantVelocityMobilityModel> cvmm;
// for (int i = 0; i < nSender; ++i) {
//     cvmm = wifiStaNodes.Get(i)->GetObject<ConstantVelocityMobilityModel>();
//    // cvmm->SetPosition(Vector(-50.0 + 10.0 * i, -50.0, 0.0)); 
//     cvmm->SetVelocity(Vector(speedOfNodes,0.0, 0.0));  // Set velocity
// }

// for (int i = 0; i < nReceiver; ++i) {
//     cvmm = wifiStaNodesRcv.Get(i)->GetObject<ConstantVelocityMobilityModel>();
//     //cvmm->SetPosition(Vector(50.0 - 10.0 * i, -50.0, 0.0));  
//     cvmm->SetVelocity(Vector(speedOfNodes, 0.0, 0.0));  // Set velocity
// }

  MobilityHelper mobility;
  // // Left side (Senders)
    mobility.SetPositionAllocator("ns3::GridPositionAllocator",
                                  "MinX",
                                  DoubleValue(0.0),
                                  "MinY",
                                  DoubleValue(0.0),
                                  "DeltaX",
                                  DoubleValue(5.0),
                                  "DeltaY",
                                  DoubleValue(5.0),
                                  "GridWidth",
                                  UintegerValue(1),
                                  "LayoutType",
                                  StringValue("RowFirst"));
    mobility.SetMobilityModel("ns3::ConstantVelocityMobilityModel");
    mobility.Install(wifiStaNodes);
    for (int i = 0; i < nSender; i++)
    {
        Ptr<ConstantVelocityMobilityModel> cvmm = wifiStaNodes.Get(i)->GetObject<ConstantVelocityMobilityModel>();
        cvmm->SetVelocity(Vector(speedOfNodes, 0, 0));
    }
    // statitc left ap node
    mobility.SetPositionAllocator("ns3::GridPositionAllocator",
                                  "MinX",
                                  DoubleValue(5.0),
                                  "MinY",
                                  DoubleValue(((nSender-1)*5.0)/2.0),
                                  "DeltaX",
                                  DoubleValue(5.0),
                                  "DeltaY",
                                  DoubleValue(5.0),
                                  "GridWidth",
                                  UintegerValue(1),
                                  "LayoutType",
                                  StringValue("RowFirst"));
    mobility.SetMobilityModel("ns3::ConstantPositionMobilityModel");
    mobility.Install(wifiApNode);
   // right side mobile receivers
    mobility.SetPositionAllocator("ns3::GridPositionAllocator",
                                  "MinX",
                                  DoubleValue(15.0),
                                  "MinY",
                                  DoubleValue(0.0),
                                  "DeltaX",
                                  DoubleValue(5.0),
                                  "DeltaY",
                                  DoubleValue(5.0),
                                  "GridWidth",
                                  UintegerValue(1),
                                  "LayoutType",
                                  StringValue("RowFirst"));
    mobility.SetMobilityModel("ns3::ConstantVelocityMobilityModel");
    mobility.Install(wifiStaNodesRcv);
    for (int i = 0; i < nReceiver; i++)
    {
        Ptr<ConstantVelocityMobilityModel> cvmm =wifiStaNodesRcv.Get(i)->GetObject<ConstantVelocityMobilityModel>();
        cvmm->SetVelocity(Vector(speedOfNodes, 0, 0));
    }
     //static right ap node
    mobility.SetPositionAllocator("ns3::GridPositionAllocator",
                                  "MinX",
                                  DoubleValue(10.0),
                                  "MinY",
                                  DoubleValue(((nReceiver-1)*5.0)/2.0),
                                  "DeltaX",
                                  DoubleValue(5.0),
                                  "DeltaY",
                                  DoubleValue(5.0),
                                  "GridWidth",
                                  UintegerValue(1),
                                  "LayoutType",
                                  StringValue("RowFirst"));
    mobility.SetMobilityModel("ns3::ConstantPositionMobilityModel");
    mobility.Install(wifiApNodeRcv);


    Ptr<RateErrorModel> em = CreateObject<RateErrorModel>();
    em->SetAttribute("ErrorRate", DoubleValue(0.0001));
    p2pDevices.Get(1)->SetAttribute("ReceiveErrorModel", PointerValue(em));

     //std::cout << "NodeCount: " << ns3::NodeList::GetNNodes() << std::endl;
    InternetStackHelper stack;
    stack.Install(wifiApNodeRcv);
    stack.Install(wifiApNode);
    stack.Install(wifiStaNodes);
    stack.Install(wifiStaNodesRcv);
    //stack.Install(p2pNodes);
   

    Ipv4AddressHelper address;
    address.SetBase("10.1.1.0", "255.255.255.0");
    Ipv4InterfaceContainer p2pInterfaces;
    p2pInterfaces = address.Assign(p2pDevices);
  
    address.SetBase("10.1.2.0", "255.255.255.0");
    address.Assign(staDevices);
    address.Assign(apDevices);

    address.SetBase("10.1.3.0", "255.255.255.0");
    Ipv4InterfaceContainer receiverInterfaces= address.Assign(staDevicesRcv);
    address.Assign(apDevicesRcv);



    Ipv4GlobalRoutingHelper::PopulateRoutingTables();
    
    //========================create receiver applications========================================
    int flowsPerNode=nFlows/(nSender);
    for(int i=0;i<nSender;i++){
        for(int j=0;j<flowsPerNode;j++)
        {
             PacketSinkHelper sinkHelper("ns3::TcpSocketFactory",(InetSocketAddress(receiverInterfaces.GetAddress(i),9+j)));
            ApplicationContainer rcvApp=sinkHelper.Install(wifiStaNodesRcv.Get(i));
            rcvApp.Start(Seconds(0.5));
            rcvApp.Stop(Seconds(simulationTimeInSeconds+0.5));
             Ptr<PacketSink> sink=StaticCast<PacketSink>(rcvApp.Get(0));
              //std::cout << "NodeCount: " << ns3::NodeList::GetNNodes() << std::endl;
             sink->TraceConnectWithoutContext("Rx", MakeCallback(&rxTracer));
             //std::cout << "eta hoose: "<< std::endl;
             
        }
    }


   // =======================Sender Application Setup:=====================================================

     for(int i=0;i<nSender;i++)
     {
        for(int j=0;j<nFlows/(nSender);j++)
        {
    OnOffHelper onoffSender("ns3::TcpSocketFactory",(InetSocketAddress(receiverInterfaces.GetAddress(i), 9+j)));
    onoffSender.SetAttribute("PacketSize", UintegerValue(1024));
    onoffSender.SetAttribute("OnTime", StringValue(
        "ns3::ConstantRandomVariable[Constant=1]"));
    onoffSender.SetAttribute("OffTime", StringValue(
        "ns3::ConstantRandomVariable[Constant=0]"));
        // std::cout << "eta hoose: "<< std::endl;
    onoffSender.SetAttribute("DataRate", DataRateValue(DataRate(datarate)));
     //std::cout << "eta hoose 2: "<< std::endl;
    ApplicationContainer senderApp = onoffSender.Install(wifiStaNodes.Get(i));
    senderApp.Start(Seconds(0.5));
    senderApp.Stop(Seconds(simulationTimeInSeconds+0.5));
     //std::cout << "eta hoose: "<< std::endl;
    Ptr<OnOffApplication> oop=StaticCast<OnOffApplication>(senderApp.Get(0));
    oop->TraceConnectWithoutContext("Tx", MakeCallback(&transmissionTracer));
     //std::cout << "eta hoose: 2 "<< std::endl;
        }
    }
     //std::cout << "eta hoose: 3"<< std::endl;
    Simulator::Stop(Seconds(simulationTimeInSeconds+1));



    Simulator::Run();
     //std::cout << "eta hoose: 4"<< std::endl;
    Simulator::Destroy();

   
// Ptr<ConstantVelocityMobilityModel> cvmm2 = wifiStaNodes.Get(nSender-2)->GetObject<ConstantVelocityMobilityModel>();

// // Connect the CourseChange trace source to the mobility model of the specific sender node
// std::ostringstream oss2;
// oss2 << "Sender_" << nSender-2;
// cvmm2->TraceConnectWithoutContext("CourseChange", MakeBoundCallback(&CourseChange, oss2.str()));

    double averageThroughput=calculateThroughput();
    double deliveryRatio=packetDeliveryRatioCalculation();



    NS_LOG_UNCOND("\t"<< averageThroughput);
    NS_LOG_UNCOND("\t"<< deliveryRatio);
    return 0;
}
