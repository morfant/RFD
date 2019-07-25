echo "Start to NTRIP cast through aws NTRIP Caster.."
#str2str -in serial://ttyUSB1:230400:8:n:1:#rtcm3 -out ntrips://:gnss@ec2-18-223-255-151.us-east-2.compute.amazonaws.com:9000/WELCOME -msg "1005(1),1077(1),1087(1),1097(1),1127(1),1230(10)"
str2str -in serial://ttyACM1:230400:8:n:1:#rtcm3 -out ntrips://:gnss@ec2-18-223-255-151.us-east-2.compute.amazonaws.com:9000/WELCOME -msg "1005(1),1077(1),1087(1),1097(1),1127(1),1230(10)"

