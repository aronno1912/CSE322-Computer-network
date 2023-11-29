#!/bin/bash
rm -f *.dat
# # Define an array with different values of nNodes
declare -a nNodesArray=("20" "40" "60" "80" "100")

# Loop over each value of nNodes and run the simulation
for nNodes in "${nNodesArray[@]}"
do
    #g++ -std=c++11 -Wall -o simulation scratch/1905053_2.cc -lns3.39-core-debug -lns3.39-csma-debug -lns3.39-network-debug -lns3.39-internet-debug -lns3.39-mobility-debug -lns3.39-point-to-point-debug -lns3.39-wifi-debug -lns3.39-applications-debug
        nF=$((nNodes / 2))
    # Run the simulation with the current value of nNodes
    ./ns3 run scratch/1905053_2 -- --nNodes=$nNodes --nFlows=$nF --nPacketsPerSecond=100 --speedOfNodes=5 --datFileTrack=1

    sleep 5
done


#Plot the graph using Gnuplot
gnuplot -persist <<-EOFMarker
    set terminal png size 640,480
    set output "throughput_vs_nNodesM.png"
    set xlabel "nNodes"
    set ylabel "Throughput (Mbit/s)"
    plot "throughput_vs_nNodesM.dat" with linespoints title "Throughput vs. nNodes"
EOFMarker

gnuplot -persist <<-EOFMarker
    set terminal png size 640,480
    set output "deliveryRatio_vs_nNodesM.png"
    set xlabel "nNodes"
    set ylabel "deliveryRatio "
    plot "deliveryRatio_vs_nNodesM.dat" with linespoints title "deliveryRatio vs. nNodes"
EOFMarker

# ##########################################   Varying nFlows    #####################################################################################
# # Define an array with different values of nFlows
declare -a nFlowsArray=("10" "20" "30" "40" "50")

# Loop over each value of nFlows and run the simulation
for nFlows in "${nFlowsArray[@]}"
do
    #g++ -std=c++11 -Wall -o simulation scratch/1905053_2.cc -lns3.39-core-debug -lns3.39-csma-debug -lns3.39-network-debug -lns3.39-internet-debug -lns3.39-mobility-debug -lns3.39-point-to-point-debug -lns3.39-wifi-debug -lns3.39-applications-debug

    # Run the simulation with the current value of nFlows
    #value of reset others are fixed
    ./ns3 run scratch/1905053_2 -- --nNodes=20 --nFlows=$nFlows --nPacketsPerSecond=100 --speedOfNodes=5 --datFileTrack=2

    # Clean up generated files
    rm -rf scratch/*.dat scratch/*.pcap

    # Optionally, you can move or save the output files for each run in separate directories
    # mkdir -p output/nFlows_$nFlows
    # mv scratch/* output/nFlows_$nFlows/

    # Wait for a few seconds between runs 
    sleep 5
done


#Plot the graph using Gnuplot
gnuplot -persist <<-EOFMarker
    set terminal png size 640,480
    set output "throughput_vs_nFlowsM.png"
    set xlabel "nFlows"
    set ylabel "Throughput (Mbit/s)"
    plot "throughput_vs_nFlowsM.dat" with linespoints title "Throughput vs. nFlows"
EOFMarker

gnuplot -persist <<-EOFMarker
    set terminal png size 640,480
    set output "deliveryRatio_vs_nFlowsM.png"
    set xlabel "nFlows"
    set ylabel "deliveryRatio "
    plot "deliveryRatio_vs_nFlowsM.dat" with linespoints title "deliveryRatio vs. nFlows"
EOFMarker
########################################### Varying Speed  #####################################################################


# # Define an array with different values of speedOfNodes
declare -a nCoverageArray=("5" "10" "15" "20" "25")

# Loop over each value of speedOfNodes and run the simulation
for speedOfNodes in "${nCoverageArray[@]}"
do
    #g++ -std=c++11 -Wall -o simulation scratch/1905053_2.cc -lns3.39-core-debug -lns3.39-csma-debug -lns3.39-network-debug -lns3.39-internet-debug -lns3.39-mobility-debug -lns3.39-point-to-point-debug -lns3.39-wifi-debug -lns3.39-applications-debug

    # Run the simulation with the current value of speedOfNodes
    #value of reset others are fixed
    ./ns3 run scratch/1905053_2 -- --nNodes=20 --nFlows=10 --nPacketsPerSecond=500 --speedOfNodes=$speedOfNodes --datFileTrack=4

    # Clean up generated files
    rm -rf scratch/*.dat scratch/*.pcap

    # Optionally, you can move or save the output files for each run in separate directories
    # mkdir -p output/speedOfNodes_$speedOfNodes
    # mv scratch/* output/speedOfNodes_$speedOfNodes/

    # Wait for a few seconds between runs 
    sleep 5
done


#Plot the graph using Gnuplot
gnuplot -persist <<-EOFMarker
    set terminal png size 640,480
    set output "throughput_vs_speedOfNodesM.png"
    set xlabel "speedOfNodes"
    set ylabel "Throughput (Mbit/s)"
    plot "throughput_vs_speedOfNodesM.dat" with linespoints title "Throughput vs. speedOfNodes"
EOFMarker

gnuplot -persist <<-EOFMarker
    set terminal png size 640,480
    set output "deliveryRatio_vs_speedOfNodesM.png"
    set xlabel "speedOfNodes"
    set ylabel "deliveryRatio "
    plot "deliveryRatio_vs_speedOfNodesM.dat" with linespoints title "deliveryRatio vs. speedOfNodes"
EOFMarker

########################################## Varying nPackets ########################################################################################

# # Define an array with different values of nPacketsPerSecond
declare -a nPacketsArray=("100" "200" "300" "400" "500")

# Loop over each value of nPacketsPerSecond and run the simulation
for nPacketsPerSecond in "${nPacketsArray[@]}"
do
    #g++ -std=c++11 -Wall -o simulation scratch/1905053_2.cc -lns3.39-core-debug -lns3.39-csma-debug -lns3.39-network-debug -lns3.39-internet-debug -lns3.39-mobility-debug -lns3.39-point-to-point-debug -lns3.39-wifi-debug -lns3.39-applications-debug

    # Run the simulation with the current value of nPacketsPerSecond
    #value of reset others are fixed
    ./ns3 run scratch/1905053_2 -- --nNodes=50 --nFlows=50 --nPacketsPerSecond=$nPacketsPerSecond --speedOfNodes=5 --datFileTrack=3

    # Clean up generated files
    rm -rf scratch/*.dat scratch/*.pcap

    # Optionally, you can move or save the output files for each run in separate directories
    # mkdir -p output/nPacketsPerSecond_$nPacketsPerSecond
    # mv scratch/* output/nPacketsPerSecond_$nPacketsPerSecond/

    # Wait for a few seconds between runs 
    sleep 5
done


#Plot the graph using Gnuplot
gnuplot -persist <<-EOFMarker
    set terminal png size 640,480
    set output "throughput_vs_nPacketsPerSecondM.png"
    set xlabel "nPacketsPerSecond"
    set ylabel "Throughput (Mbit/s)"
    plot "throughput_vs_nPacketsPerSecondM.dat" with linespoints title "Throughput vs. nPacketsPerSecond"
EOFMarker

gnuplot -persist <<-EOFMarker
    set terminal png size 640,480
    set output "deliveryRatio_vs_nPacketsPerSecondM.png"
    set xlabel "nPacketsPerSecond"
    set ylabel "deliveryRatio "
    plot "deliveryRatio_vs_nPacketsPerSecondM.dat" with linespoints title "deliveryRatio vs. nPacketsPerSecond"
EOFMarker