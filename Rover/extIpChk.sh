filename=extIp_`date +%Y%m%d_%X`.log;
while true;
do echo "           "
date +%X >> ./log/$filename; 
curl icanhazip.com >> ./log/$filename; 
echo -e "\n" >> ./log/$filename;
sleep 10;
done

