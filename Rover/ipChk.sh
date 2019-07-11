filename=log_`date +%Y%m%d_%X`.log;
while true;
do echo "           "
date +%X >> $filename; 
curl icanhazip.com >> $filename; 
echo -e "\n" >> $filename;
sleep 10;
done

