echo "Start to NTRIP cast through aws NTRIP Caster.."
str2str -in serial://ttyUSB0:115200:8:n:1:#rtcm3 -out tcpsvr://localhost:30000 -msg "1005(1),1074(1),1084(1),1094(1),1124(1),1230(10)"
