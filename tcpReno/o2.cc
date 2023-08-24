//started ns3 offline with resourceeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeee
#include "ns3/core-module.h"
#include "ns3/applications-module.h"
#include "ns3/csma-module.h"
#include "ns3/internet-module.h"
#include "ns3/mobility-module.h"
#include "ns3/network-module.h"
#include "ns3/point-to-point-layout-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/stats-module.h"
#include "ns3/callback.h"
#include "ns3/flow-monitor-module.h"
#include "ns3/ssid.h"

using namespace ns3;
using namespace std;

int simulationTime = 60;


//This custom application is responsible for sending packets from the sender nodes to the corresponding receiver nodes.

class MyTutorialApp : public Application
{


private:
  virtual void StartApplication (void);
  virtual void StopApplication (void);

  void ScheduleTx (void);
  void SendPacket (void);

  Ptr<Socket>     m_socket;
  Address         m_peer;
  uint32_t        m_packetSize;
  DataRate        m_dataRate;
  EventId         m_sendEvent;
  bool            m_running;
  uint32_t        m_packetsSent;
  uint32_t        m_simultime;

  public:
  MyTutorialApp ();
  virtual ~MyTutorialApp ();
  void Setup (Ptr<Socket> socket, Address address, uint32_t packetSize, DataRate dataRate, uint32_t simultime);
   static TypeId GetTypeId (void);
};

MyTutorialApp::MyTutorialApp ()
  : m_socket (0),
    m_peer (),
    m_packetSize (0),
    m_dataRate (0),
    m_sendEvent (),
    m_running (false),
    m_packetsSent (0),
    m_simultime (0)
{
}

MyTutorialApp::~MyTutorialApp ()
{
  m_socket = 0;
}

/* static */
TypeId MyTutorialApp::GetTypeId (void)
{
    static TypeId tid = TypeId("MyTutorialApp")
                            .SetParent<Application>()
                            .SetGroupName("Tutorial")
                            .AddConstructor<MyTutorialApp>();
    return tid;
}
//added simulTime here
void
MyTutorialApp::Setup (Ptr<Socket> socket, Address address, uint32_t packetSize, DataRate dataRate, uint32_t simultime)
{
  m_socket = socket;
  m_peer = address;
  m_packetSize = packetSize;
  m_dataRate = dataRate;
  m_simultime = simultime;
}

void
MyTutorialApp::StartApplication (void)
{
   m_running = true;
    m_packetsSent = 0;
    m_socket->Bind();
    m_socket->Connect(m_peer);
    SendPacket();
}

void
MyTutorialApp::StopApplication (void)
{
  m_running = false;

  if (m_sendEvent.IsRunning ())
    {
      Simulator::Cancel (m_sendEvent);
    }

  if (m_socket)
    {
      m_socket->Close ();
    }
}

/***The purpose of this method is to send packets to the destination address using the socket, and if the specified simulation time has not been 
 * reached, it schedules the next packet transmission. This mechanism ensures that the application keeps sending packets until 
 * the specified simulation time is reached. m_simultime is the specified simulation time until which the application should continue sending packets.
*/

void
MyTutorialApp::SendPacket (void)
{
  Ptr<Packet> packet = Create<Packet> (m_packetSize);
  m_socket->Send (packet);

  if(Simulator::Now().GetSeconds() < m_simultime) 
  ScheduleTx();
}

// is responsible for scheduling the next packet transmission using the Simulator
//the purpose of the ScheduleTx method is to calculate the time interval for the next packet transmission based on the packet size and data rate,
// and then schedule the SendPacket method to be called after that interval. This mechanism ensures a controlled and continuous packet transmission process 
//for the duration that the application is active (m_running is true).
void
MyTutorialApp::ScheduleTx (void)
{
  if (m_running)
    {
      Time tNext (Seconds (m_packetSize * 8 / static_cast<double> (m_dataRate.GetBitRate ())));
      m_sendEvent = Simulator::Schedule (tNext, &MyTutorialApp::SendPacket, this);
    }
}

// callback that gets called whenever there's a change in the congestion window (cwnd) of a TCP connection.
static void
CongestionWindowTracer (Ptr<OutputStreamWrapper> stream, uint32_t oldCwnd, uint32_t newCwnd)
{
  *stream->GetStream () << Simulator::Now ().GetSeconds () << " " << newCwnd << std::endl;
}


int main(int argc, char *argv[]) {
    // Enable logging
    //LogComponentEnable("PointToPointDumbbellHelper", LOG_LEVEL_INFO);

    // Config::SetDefault("ns3::TcpL4Protocol::SocketType", TypeIdValue(TcpNewReno::GetTypeId()));
    // Config::SetDefault("ns3::TcpSocket::SegmentSize", UintegerValue(1024));
    // Config::SetDefault("ns3::OnOffApplication::PacketSize", UintegerValue(1024));

    // Define simulation parameters
    uint32_t nNodes = 4; // Total number of nodes in the dumbbell topology
    uint32_t bottleneckDataRate=50; 
     uint32_t bottleneckDelay=100;
    int dataTrack=0;
    double packetLossRate=1e6;
    std::string pointToPointLeafLinkBw = "1Gbps"; //senderDataRate
    std::string pointToPointLeafLinkDelay = "1ms"; //senderDelay
    
   
    uint32_t payloadSize = 1472; // jania
    std::string tcpVariant1 = "ns3::TcpNewReno"; // TcpNewReno
    std::string tcpVariant2 = "ns3::TcpHighSpeed"; //TcpAdaptiveReno TcpWestwood, TcpHighSpeed
     int nFlows = 2; 
    std::string output_folder = "GeneratedFiles/tcphs";
    int cleanupTime = 2;
    int packet_loss_exp = 6; //packet loss rate for the error model constant at 10−6
    int exp = 1; //1 for bttlnck, 2 for packet loss rate
    //flow monitor stats
    float avgTp = 0;
    float currTp = 0;
    float currTpArr[] = {0, 0}; //for two flows
    double sumOfXi = 0;
    double sumOfXiSquare = 0;
    // variables for output measurement
    uint32_t SentPackets = 0;
    uint32_t ReceivedPackets = 0;
    uint32_t LostPackets = 0;

     // input from CMD
    CommandLine cmd (__FILE__);
    //cmd.AddValue ("tcp2","Name of TCP variant 2", tcpVariant2);
    cmd.AddValue ("bottleneckDataRate","Max Packets allowed in the device queue", bottleneckDataRate);
    cmd.AddValue ("packet_loss_exp", "Packet loss rate exponent", packet_loss_exp);
    cmd.AddValue ("dataTrack", "Which parameter is varrying(1 for dataRate ,2 for packect loss)", dataTrack);
    //cmd.AddValue ("output_folder","Folder to store dara", output_folder);
    //cmd.AddValue ("exp","1 for bttlnck, 2 for packet loss rate", exp);
    cmd.Parse (argc,argv);
    std::string bottleNeckLinkBw = std::to_string(bottleneckDataRate)+"Mbps";
    std::string bottleNeckLinkDelay = std::to_string(bottleneckDelay)+"ms";
    std::string file = output_folder + "/data_" + std::to_string(exp) + ".txt";
    double packet_loss_rate = (1.0 / std::pow(10, packet_loss_exp)); //10^-6
     double bandwidth_delay_product = bottleneckDelay*bottleneckDataRate;
    
   Config::SetDefault ("ns3::TcpSocket::SegmentSize", UintegerValue (payloadSize));
  
  //=================================== Topology Creation =================================================================
    PointToPointHelper bottleneckHelper;
    bottleneckHelper.SetDeviceAttribute("DataRate", StringValue(bottleNeckLinkBw));
    bottleneckHelper.SetChannelAttribute("Delay", StringValue(bottleNeckLinkDelay));


    PointToPointHelper pointToPointLeaf;
    pointToPointLeaf.SetDeviceAttribute("DataRate", StringValue(pointToPointLeafLinkBw));
    pointToPointLeaf.SetChannelAttribute("Delay", StringValue(pointToPointLeafLinkDelay));

    // add router buffer capacity - equal to bandwidth delay product
    pointToPointLeaf.SetQueue ("ns3::DropTailQueue", "MaxSize", StringValue (std::to_string (bandwidth_delay_product) + "p"));

    PointToPointDumbbellHelper dumbbell(2, pointToPointLeaf, 2, pointToPointLeaf, bottleneckHelper);

    //========= setting error model =======================================
   //error model is set on the receive side of the router device of the bottleneck link
    Ptr<RateErrorModel> em = CreateObject<RateErrorModel> ();
    em->SetAttribute ("ErrorRate", DoubleValue (packet_loss_rate));
    dumbbell.m_routerDevices.Get(1)->SetAttribute ("ReceiveErrorModel", PointerValue (em)); 

    //  InternetStackHelper stackNewReno;

    // stackNewReno.Install(dumbbell.GetLeft(0));
    // stackNewReno.Install(dumbbell.GetRight(0));

    // InternetStackHelper stackWestwoodPlus;
    // stackWestwoodPlus.Install(dumbbell.GetLeft(1));
    // stackWestwoodPlus.Install(dumbbell.GetRight(1));
    // stackNewReno.Install(dumbbell.GetLeft());
    // stackNewReno.Install(dumbbell.GetRight());


    // ================================ Stack Installation ===============================================================
    //each pair of nodes can use a different TCP congestion control algorithm


    // tcp variant 1
    Config::SetDefault ("ns3::TcpL4Protocol::SocketType", StringValue (tcpVariant1)); //tcpnewReno
    InternetStackHelper stack1;
        stack1.Install (dumbbell.GetLeft (0));
        stack1.Install (dumbbell.GetRight (0)); 
      
        stack1.Install (dumbbell.GetLeft ());
        stack1.Install (dumbbell.GetRight ());

    // tcp variant 2
    Config::SetDefault ("ns3::TcpL4Protocol::SocketType", StringValue (tcpVariant2));
    InternetStackHelper stack2;
        stack2.Install (dumbbell.GetLeft (1)); 
        stack2.Install (dumbbell.GetRight (1)); 



   //=============== ip address assignment ======================================================================================================
    dumbbell.AssignIpv4Addresses (Ipv4AddressHelper ("10.1.1.0", "255.255.255.0"), // left leafs
                          Ipv4AddressHelper ("10.2.1.0", "255.255.255.0"),  // right leaves
                          Ipv4AddressHelper ("10.3.1.0", "255.255.255.0")); // routers 
    Ipv4GlobalRoutingHelper::PopulateRoutingTables (); // populate routing table
    

    // ========================== installation and configuration of the flow monitor =====================================================================
    FlowMonitorHelper flowmonitorHelper;
    flowmonitorHelper.SetMonitorAttribute("MaxPerHopDelay", TimeValue(Seconds(2)));
    Ptr<FlowMonitor> monitor = flowmonitorHelper.InstallAll (); //Enable flow monitoring on all nodes

    uint16_t portNum = 8080;
    
    for(int i=0;i<nFlows; i++)
    {
      // ================================ Setting Up Packet Sink (Receiver)==========================================
      Address sinkAddress (InetSocketAddress (dumbbell.GetRightIpv4Address (i), portNum));
      PacketSinkHelper packetSinkHelper ("ns3::TcpSocketFactory", InetSocketAddress (Ipv4Address::GetAny(), portNum));
      ApplicationContainer sinkApps = packetSinkHelper.Install (dumbbell.GetRight (i));
      sinkApps.Start (Seconds (0));
      sinkApps.Stop (Seconds (simulationTime+2));

      // ========================= Setting Up Sender Application (MyTutorialApp)==========================================

      Ptr<Socket> ns3TcpSocket = Socket::CreateSocket (dumbbell.GetLeft (i), TcpSocketFactory::GetTypeId ());
      Ptr<MyTutorialApp> app = CreateObject<MyTutorialApp> ();
      app->Setup (ns3TcpSocket, sinkAddress, payloadSize, DataRate (pointToPointLeafLinkBw), simulationTime);
      dumbbell.GetLeft (i)->AddApplication (app);
      app->SetStartTime (Seconds (1));
      app->SetStopTime (Seconds (simulationTime));


     //====================  Trace Connection for Congestion Window ======================

      std::ostringstream oss; // output stream is created for tracing the congestion window changes
      oss << output_folder << "/flow" << i+1 <<  ".cwnd";
      AsciiTraceHelper asciiTraceHelper;
      Ptr<OutputStreamWrapper> stream = asciiTraceHelper.CreateFileStream (oss.str());
      ns3TcpSocket->TraceConnectWithoutContext ("CongestionWindow", MakeBoundCallback (&CongestionWindowTracer, stream));
    }

    Simulator::Stop (Seconds (simulationTime+2));
    Simulator::Run ();

  // processing the statistics collected by the Flow Monitor and performing calculations related to network performance metrics
    int j = 0;
    std::ofstream MyFile(file, std::ios_base::app);
    // =================================two dat files for each graph for two different congestion control algo==========================
     std::ofstream bneckFile1("GeneratedFiles/tcphs/throughput_vs_bottoleNeckDataRate1.dat", std::ios::app);  //for varying bottoleNeckDataRate
      std::ofstream packLossFile1("GeneratedFiles/tcphs/throughput_vs_packetLossRatio1.dat", std::ios::app);  //for varying packetLossRatio
     std::ofstream cwdFile1("GeneratedFiles/tcphs/CongestionWindowVSTime1.dat", std::ios::app);  
      std::ofstream bneckFile2("GeneratedFiles/tcphs/throughput_vs_bottoleNeckDataRate2.dat", std::ios::app);  //for varying bottoleNeckDataRate
      std::ofstream packLossFile2("GeneratedFiles/tcphs/throughput_vs_packetLossRatio2.dat", std::ios::app);  //for varying packetLossRatio
     std::ofstream cwdFile2("GeneratedFiles/tcphs/CongestionWindowVSTime2.dat", std::ios::app);  
    //Ptr<Ipv4FlowClassifier> classifier = DynamicCast<Ipv4FlowClassifier> (flowmonitorHelper.GetClassifier ());

    //Get the statistics container (FlowStatsContainer) from the FlowMonitor. It holds flow statistics for various flows monitored during the simulation.
    // signature is  typedef std::map<FlowId, FlowStats> FlowStatsContainer;
    FlowMonitor::FlowStatsContainer statsContainer = monitor->GetFlowStats ();

       for (auto iter = statsContainer.begin (); iter != statsContainer.end (); ++iter) 
      {
        //current throughput calculation in Kbps
      currTp = iter->second.rxBytes * 8.0/((simulationTime+cleanupTime)*1000);
   
      if(j%2 == 0) 
      { currTpArr[0] += iter->second.rxBytes; }
      if(j%2 == 1) 
      { currTpArr[1] += iter->second.rxBytes; }
      SentPackets = SentPackets +(iter->second.txPackets);
      ReceivedPackets = ReceivedPackets + (iter->second.rxPackets);
      LostPackets = LostPackets + (iter->second.lostPackets);
      j = j + 1;
      sumOfXi += currTp;   //jain_index numerator
      sumOfXiSquare += (currTp * currTp); //jain_index denominator
    }
    
    //++++++++++++++++++++ Calculate the Jain’s Fairness Index +++++++++++++++++++++++++++++++++++++++++=
   // jain index= (sum of xi)^2/(n * sum of xi^2)
     double jain_index = (sumOfXi * sumOfXi) / ( j * sumOfXiSquare);
    currTpArr[0] /= ((simulationTime+ cleanupTime)*1000);
    currTpArr[1] /= ((simulationTime+ cleanupTime)*1000);
    avgTp = currTpArr[0] + currTpArr[1];

    //============= write results in .dat files for graph plotting ++++++++++===============================
    if(dataTrack==1)
    {bneckFile1 << bottleneckDataRate << "\t" << currTpArr[0] << std::endl;
     bneckFile2 << bottleneckDataRate << "\t" << currTpArr[1] << std::endl;}
     if(dataTrack==2)
     {
    packLossFile1 << packet_loss_rate << "\t" << currTpArr[0] << std::endl;
     packLossFile2 << packet_loss_rate << "\t" << currTpArr[1] << std::endl;
     }
    // cols : bttlneck rate | random packet loss | throughput1 | throughput2 | jain_index
    MyFile << bottleNeckLinkBw.substr(0, bottleNeckLinkBw.length()-4) << " " << -1*packet_loss_exp << " " << currTpArr[0] << " " << currTpArr[1] << " " << " " << jain_index <<std::endl;
    NS_LOG_UNCOND("\n--------Total Results of the simulation----------"<<std::endl);
    NS_LOG_UNCOND("Total sent packets  = " << SentPackets);
    NS_LOG_UNCOND("Total Received Packets = " << ReceivedPackets);
    NS_LOG_UNCOND("Total Lost Packets = " << LostPackets);
    NS_LOG_UNCOND("Packet Loss ratio = " << ((LostPackets*100.00)/SentPackets)<< "%");
    NS_LOG_UNCOND("Packet delivery ratio = " << ((ReceivedPackets*100.00)/SentPackets)<< "%");
    NS_LOG_UNCOND("Average Throughput = " << avgTp<< "Kbps");
    NS_LOG_UNCOND("Jain's Fairness Index = " << jain_index);

   
    
    Simulator::Destroy ();
    return 0;

}
