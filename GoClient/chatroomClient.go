package main

import ("fmt"
		"net"
		"bufio"
		"os"
)

type Client struct {
	conn net.Conn
	name string
}

func main() {
	connection, e := net.Dial("tcp", "127.0.0.1:8888");
	HandleError(e,"Dial");
	fmt.Println("Input your name:");
	reader := bufio.NewReader(os.Stdin);
	lineBytes, _, _ := reader.ReadLine();
	fmt.Println("Read ok",string(lineBytes));
	go handleReceive(Client{connection,string(lineBytes)});
	handleSend(Client{connection,string(lineBytes)});
}
func HandleError(err error,why string){
	if err!=nil{
		fmt.Println(why+"fail");
		os.Exit(1);
	}
	fmt.Println(why," ok");
}

func handleSend(client Client){
	fmt.Println("begin to write");
	_,e:=client.conn.Write([]byte(client.name));
	HandleError(e,"Write");
	reader:=bufio.NewReader(os.Stdin);
	for {
		lineBytes, _, _ := reader.ReadLine();

		client.conn.Write(lineBytes);
		if string(lineBytes)=="exit"{
			os.Exit(0);
		}
	}
}
func handleReceive(client Client){
	buffer:=make([]byte,500);
	for{
		n,_:=client.conn.Read(buffer);
		if n>0{
			msg:=string(buffer[:n]);
			fmt.Println(msg);
		}
	}
}