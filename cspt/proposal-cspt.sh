echo "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~"
echo "~~~~~~~~~~~~~~~~~~$1 has been started.~~~~~~~~~~~~~~~"
echo "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~"


dir=`pwd`

echo $dir

if [ "$2" = "0" ]
then
	cp carrier_0.input.request original_requests.txt
#       cp carrier_0.input.request 1/original_requests.txt
#	cp carrier_0.input.request 2/original_requests.txt
#	cp carrier_0.input.request 3/original_requests.txt
#	cp carrier_0.input.request 4/original_requests.txt
fi

if [ "$2" = "1" ]
then
	cp carrier_1.input.request original_requests.txt
#       cp carrier_1.input.request 1/original_requests.txt
#	cp carrier_1.input.request 2/original_requests.txt
#	cp carrier_1.input.request 3/original_requests.txt
#	cp carrier_1.input.request 4/original_requests.txt
fi

if [ "$2" = "2" ]
then
	cp carrier_2.input.request original_requests.txt
#       cp carrier_2.input.request 1/original_requests.txt
#	cp carrier_2.input.request 2/original_requests.txt
#	cp carrier_2.input.request 3/original_requests.txt
#	cp carrier_2.input.request 4/original_requests.txt
fi


#cp 0* 1/
#cp 0* 2/
#cp 0* 3/
#cp 0* 4/

#cp cplex-cspt.sh 1/
#cp cplex-cspt.sh 2/
#cp cplex-cspt.sh 3/
#cp cplex-cspt.sh 4/

#cp rcon* 1/
#cp rcon* 2/
#cp rcon* 3/
#cp rcon* 4/

#cp $1.01.input.conf 1/$1.01.input.conf
#cp $1.01.input.price 1/$1.01.input.price
#cp $1.01.input.request 1/$1.01.input.request

#cp $1.02.input.conf 2/$1.02.input.conf
#cp $1.02.input.price 2/$1.02.input.price
#cp $1.02.input.request 2/$1.02.input.request

#cp $1.03.input.conf 3/$1.03.input.conf
#cp $1.03.input.price 3/$1.03.input.price
#cp $1.03.input.request 3/$1.03.input.request

#cp $1.04.input.conf 4/$1.04.input.conf
#cp $1.04.input.price 4/$1.04.input.price
#cp $1.04.input.request 4/$1.04.input.request

sh cplex-cspt.sh $1 $2 $3

#cd $dir/1
#sh cplex-cspt.sh $1.01 $2 $3
#cd $dir/2
#sh cplex-cspt.sh $1.02 $2 $3
#cd $dir/3
#sh cplex-cspt.sh $1.03 $2 $3
#cd $dir/4
#sh cplex-cspt.sh $1.04 $2 $3

#ssh 192.168.50.109 "cd $dir/1 && sh cplex-cspt.sh $1.01 $2 $3" &
#ssh 192.168.50.110 "cd $dir/2 && sh cplex-cspt.sh $1.02 $2 $3" &
#ssh 192.168.50.111 "cd $dir/3 && sh cplex-cspt.sh $1.03 $2 $3" &
#ssh 192.168.50.112 "cd $dir/4 && sh cplex-cspt.sh $1.04 $2 $3" &

cd $dir

#wait
#cp 1/*.output.* ./
#cp 2/*.output.* ./
#cp 3/*.output.* ./
#cp 4/*.output.* ./


echo "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~"
echo "~~~~~~~~~~~~~~~~~~$1 has been done.~~~~~~~~~~~~~~~~~~"
echo "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~"

exit
