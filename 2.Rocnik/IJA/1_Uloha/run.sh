if [ "$#" -eq 0 ]; then
	rm -f ija/ija2016/homework1/cardpack/*.class ija/ija2016/homework1/.*class
	javac ija/ija2016/homework1/cardpack/*.java ija/ija2016/homework1/HomeWork1.java
	java -ea ija.ija2016.homework1.HomeWork1
else
	if [ "$#" -eq 1 ] && [ "$1" == "clear" ]; then
		rm -f ija/ija2016/homework1/cardpack/*.class ija/ija2016/homework1/.*class		
	fi
fi
