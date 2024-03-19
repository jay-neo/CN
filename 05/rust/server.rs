use std::io::{Read, Write};
use std::net::{TcpListener, TcpStream};

fn handle_client(mut stream: TcpStream) {

    let peer_addr = stream.peer_addr().expect("Failed to get peer address");
    println!("Client connected from {}", peer_addr);

    loop {
        // this line reads data from the stream and stores it in the buffer.
        let mut buffer = [0; 1024];
        match stream.read(&mut buffer) {
            Ok(bytes_received) => {
                if bytes_received == 0 {
                    println!("Client disconnected from {}", peer_addr);
                    return;
                }
                 // this line converts the data in the buffer into a UTF-8 enccoded string.
                let request = String::from_utf8_lossy(&buffer[..bytes_received]);
                println!("Client {} Request = {}", peer_addr, request);
                stream.write(request.as_bytes()).expect("Failed to write response!");

                if request.trim() == "stop" {
                    println!("Client disconnected from {}", peer_addr);
                    return;
                }
            }
            Err(err) => {
                eprintln!("Client {} Error receiving data: {}", peer_addr, err);
                return;
            }
        }

        // buffer = [0; 1024];
        // stream.read(&mut buffer).expect("Failed to read from client!");
        // let request = String::from_utf8_lossy(&buffer[..]);
        // println!("Received request: {}", request);
        // let response = "Hello, Client!".as_bytes();
        // stream.write(response).expect("Failed to write response!");

    }
    
}


fn main() {
    let ip = "127.0.0.1";
    let port = 8080;
    let address = format!("{}:{}", ip, port);
    
    let listener = TcpListener::bind(&address).expect("Failed to bind to address");
    println!("Server listening on {}", address);

    for stream in listener.incoming(){
        match stream{
            Ok(stream) => {
                std::thread::spawn(|| handle_client(stream));
            }
            Err(e) => {
                eprintln!("Failed to establish connection: {}", e);
            }
        }
    }
}