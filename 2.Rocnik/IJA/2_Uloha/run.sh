if [ "$#" -eq 0 ]; then
	rm -f ija/ija2016/homework2/model/board/*.class ija/ija2016/homework2/model/cards/*.class ija/ija2016/homework2/*.class
	javac -cp .:./junit-4.12.jar:./hamcrest-core-1.3.jar ija/ija2016/homework2/model/board/*.java ija/ija2016/homework2/model/cards/*.java ija/ija2016/homework2/HomeWork2Test.java
	java -cp .:./junit-4.12.jar:./hamcrest-core-1.3.jar org.junit.runner.JUnitCore ija.ija2016.homework2.HomeWork2Test
else
	if [ "$#" -eq 1 ] && [ "$1" == "clear" ]; then
		rm -f ija/ija2016/homework2/model/board/*.class ija/ija2016/homework2/model/cards/*.class ija/ija2016/homework2/*.class
	fi
fi
