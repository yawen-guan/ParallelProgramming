cd code
bash ./run.sh 

cd ../checkcode
mvn clean package
java -jar ./target/CheckService-1.0-SNAPSHOT.jar < ../code/multithread.ll