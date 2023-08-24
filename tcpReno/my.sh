#!/bin/bash
rm -f GeneratedFiles/tcphs/*.dat
rm -f GeneratedFiles/tcphs/*.cwnd

#########################################   Varying bottleneckDataRate    #####################################################################################

declare -a bottleNeckarray=("1" "50" "100" "200" "300")

# Loop over each value of nFlows and run the simulation
for nF in "${bottleNeckarray[@]}"
do
    ./ns3 run scratch/o2 -- --bottleneckDataRate=$nF --packet_loss_exp=6 --dataTrack=1
    
    sleep 5

    # Generate graph for each flow's Congestion Window vs. Time
    gnuplot -persist <<-EOFMarker
        set terminal png size 640,480
        set output "GeneratedFiles/tcphs/cwndForBottle_${nF}.png"
        set xlabel "Time (s)"
        set ylabel "Congestion Window"
        plot "GeneratedFiles/tcphs/flow1.cwnd" with lines title "TCPNewReno", \
             "GeneratedFiles/tcphs/flow2.cwnd" with lines title "TCPHighSpeed"
EOFMarker
done


#Plot the graph using Gnuplot
gnuplot -persist <<-EOFMarker
    set terminal png size 640,480
    set output "throughput_vs_bottoleNeckDataRate.png"
    set xlabel "bottoleNeckDataRate(Mbps)"
    set ylabel "Throughput (Mbit/s)"
     plot "GeneratedFiles/tcphs/throughput_vs_bottoleNeckDataRate1.dat" with linespoints title "TcpNewReno" lc rgb 'blue', \
         "GeneratedFiles/tcphs/throughput_vs_bottoleNeckDataRate2.dat" with linespoints title "TcpHighSpeed" lc rgb 'red'
EOFMarker


##########################################   Varying packetlossRate    #####################################################################################

declare -a packExp=("2" "3" "4" "5" "6")

# Loop over each value of nFlows and run the simulation
for nF in "${packExp[@]}"
do
    ./ns3 run scratch/o2 -- --bottleneckDataRate=50 --packet_loss_exp=$nF --dataTrack=2
    sleep 5
    gnuplot -persist <<-EOFMarker
        set terminal png size 640,480
        set output "GeneratedFiles/tcphs/cwndForPacketLoss_${nF}.png"
        set xlabel "Time (s)"
        set ylabel "Congestion Window"
        plot "GeneratedFiles/tcphs/flow1.cwnd" with lines title "TCPNewReno", \
             "GeneratedFiles/tcphs/flow2.cwnd" with lines title "TCPHighSpeed"
EOFMarker
done


#Plot the graph using Gnuplot
gnuplot -persist <<-EOFMarker
    set terminal png size 640,480
    set output "throughput_vs_packetLossrate.png"
    set xlabel "PacketLossRate"
    set ylabel "Throughput (Mbit/s)"
     set logscale x  # Set x-axis to logarithmic scale
    set xrange [1e-6:1e-2]  # Set the x-axis range from 10^-6 to 10^-2
     plot "GeneratedFiles/tcphs/throughput_vs_packetLossRatio1.dat" with linespoints title "TcpNewReno" lc rgb 'blue', \
         "GeneratedFiles/tcphs/throughput_vs_packetLossRatio2.dat" with linespoints title "TcpHighSpeed" lc rgb 'red'
EOFMarker


