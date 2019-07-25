package main

import (
	"fmt"
	"net"
	"os"
)
//1.建立监听
//2.接入所有用户，保存昵称
//3.每个线程对每个用户接受输入并广播（name#content, eg:all#hello）
type Client1 struct {
	name string;
	pw string;
}
type Client2 struct{
	conn net.Conn;
	name string;
	lock bool;
}
var clientsMap =make(map[string]Client1);   //登录时使用
var onlineclients=make(map[string]Client2); //聊天时使用

func HandleError(err error,why string){
	if err!=nil{
	fmt.Println(why+"fail",err);
	os.Exit(1);
	}
	fmt.Println(why+" ok");
}

func main() {
	listener,e := net.Listen("tcp", "0.0.0.0:6666");
	HandleError(e,"listen");
	for {  //接入所有人的循环，每个人接受Acept,RomoteAddr,name后保存，自己打印，向所有人广播;之后开一个线程等待其发消息
		conn, e := listener.Accept();
		HandleError(e,"Accept");
		go serveIt(conn);
	}
}
func serveIt(conn net.Conn){
	buffer := make([]byte, 500);
	var logged bool =false;
	var client Client2;
	client.conn=conn;
	client.lock=false;
	for {      //循环等待客户端请求

		fmt.Println("Waiting for request");
		for client.lock{
			;
		}
		n, _ := conn.Read(buffer);
		if logged {
			client.lock=true;
			onlineclients[client.name] = client;
		}
		choice:=string(buffer[:n]);
		if choice=="login" {
			if(logged){
				conn.Write([]byte("0"));
				fmt.Println("Already logged in");
			}else{
				fmt.Println("登录服务");
				conn.Write([]byte("1"));  //"1"表示合法命令，"0"表示非法命令或者需要登录后使用
				logged=handleLogin(&client);
			}
		}else if choice=="reg" {
			if(logged){
				conn.Write([]byte("0"));
				fmt.Println("Already logged in");
			}else {
				fmt.Println("注册服务");
				conn.Write([]byte("1")); //"1"表示合法命令，"0"表示非法命令或者需要登录后使用
				handleRegister(conn);
			}
		}else if  choice=="chat" || choice=="img"{
			if(logged){
				fmt.Println("聊天/传文件服务");
				conn.Write([]byte("1"));
				ioWithIt(client,choice);
			}else{
				conn.Write([]byte("0"));
				fmt.Println("Log in first");
			}
		}else if choice=="exit"{ //不单独写函数了，直接实现
			if logged {
				for _, cli := range onlineclients {
					if cli == client {
						delete(clientsMap, cli.name);
					} else {
						cli.conn.Write([]byte(client.name + "log out"));
					}
				}
				fmt.Println(client.name+"log out");
			}    //这个客户端没了，锁不锁均可
			return;
		}else{
			fmt.Println("非法请求，暂不能服务:",choice);;
			conn.Write([]byte("0"));
		}
		if logged {
			client.lock=true;
			onlineclients[client.name] = client;
		}
	}
}
func handleRegister(conn net.Conn) bool {
	buffer := make([]byte, 500);
	n, _ := conn.Read(buffer);
	ID := string(buffer[:n]);
	fmt.Println("ID=", ID, "length=", len(ID));;
	_, exi := clientsMap[ID];
	if (exi) {
		fmt.Println("User exits");
		conn.Write([]byte("0"));
		return false;
	} else {
		fmt.Println("New User");
		conn.Write([]byte("1"));
		n, _ = conn.Read(buffer);
		password := string(buffer[:n]);
		clientsMap[ID] = Client1{ID, password};
		fmt.Println("Registration successful");
		return true;
	}
}

func handleLogin(client *Client2) bool{
	buffer:=make([]byte,500);
	n,_:=(*client).conn.Read(buffer);
	ID:=string(buffer[:n]);
	fmt.Println("ID=",ID,"length=",len(ID));
	if ID=="Tourist"{  //游客登录
		(*client).conn.Write([]byte("1"));
		n,_=(*client).conn.Read(buffer);
		ID=string(buffer[:n]);
		(*client).name=ID;
		onlineclients[ID] = *client;
		var msg string=ID+" log in";
		fmt.Println(msg);     //服务器端显示，加入map,并向用户广播
		/*for _,client :=range onlineclients{
			if client.name!=ID {  //对刚上线的本人不发消息
				client.conn.Write([]byte(msg));
			}
		}  //上线提示要修改*/
		return true;
	}else {
		client1, exi := clientsMap[ID];
		if (exi) {
			(*client).conn.Write([]byte("1"));
			fmt.Println("User exist");
			n, _ = (*client).conn.Read(buffer);
			password := string(buffer[:n]);
			if (password == client1.pw) {
				(*client).conn.Write([]byte("1"));
				(*client).name = ID;
				onlineclients[ID] = *client;
				var msg string = ID + " log in";
				fmt.Println(msg); //服务器端显示，加入map,并向用户广播
				for _, client := range onlineclients {
					if client.name != ID { //对刚上线的本人不发消息
						client.conn.Write([]byte(msg));
					}
				}
				return true;
			} else {
				fmt.Println("password wrong");
				(*client).conn.Write([]byte("0"));
				return false;
			}
		} else {
			fmt.Println("User not exits");
			(*client).conn.Write([]byte("0"));
			return false;
		}
	}
}

func ioWithIt(client Client2,choice string){
	buffer:=make([]byte,60000);
	n,_:= client.conn.Read(buffer);
	targetName :=string(buffer[:n]);
	fmt.Println("targetName=",targetName);

	clien, ok := onlineclients[targetName];
	if targetName=="All" || ok {
		fmt.Println("Begin to read buffer");

		client.conn.Write([]byte("1"));
		n, _ = client.conn.Read(buffer);
		fmt.Println("buffer=",string(buffer[:n]));
		fmt.Println("发送的信息格式：",client.name,"  ",choice,"  ",buffer);
		if targetName == "All" {
			fmt.Println("Send to all");
			for _, cli := range onlineclients {
				if cli.name!=client.name {
					fmt.Println("begin to send 1 user,name=", cli.name)
					for cli.lock{
						;
					}
					cli.lock=true;
					//onlineclients[cli.name]=cli;
					sendMsg(client.name, choice, buffer, cli.conn);
					cli.lock=false;
					//onlineclients[cli.name]=cli;
					fmt.Println("finish send 1 user:", client.name, " ", choice, " ", buffer);
					fmt.Println(cli.name);
				}
			}
		}else if clien.name!=client.name{
			fmt.Println("Send to one","   to:",clien.name);
			for clien.lock{
				;
			}
			clien.lock=true;
			//onlineclients[clien.name]=clien;
			sendMsg(client.name,choice,buffer,clien.conn);
			clien.lock=false;             //自己的消息不发给自己
			//onlineclients[clien.name]=clien;
		}
		fmt.Println("Send to self","   to:",client.name);
		sendMsg(client.name,choice,buffer,client.conn);
	}else {
		client.conn.Write([]byte("0"));
		fmt.Println("Not online");
	}
}
func sendMsg(name ,choice string,msg []byte,conn net.Conn){
	buff:=make([]byte,500);
	conn.Write([]byte(choice));
	conn.Read(buff);
	fmt.Println("sendMsg: choice=",choice);
	conn.Write([]byte(name));
	conn.Read(buff);
	fmt.Println("sendMsg:name:",name);
	conn.Write(msg);
	fmt.Println("sendMsg:send finished");
}