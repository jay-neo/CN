use std::io::{self, Write};
use std::net::{UdpSocket};

const SERVER_IP: &str = "127.0.0.1";
const SERVER_PORT: u16 = 8080;

fn main() {
    let server_addr = format!("{}:{}", SERVER_IP, SERVER_PORT);

    let socket = UdpSocket::bind("0.0.0.0:0").expect("Failed to connect to server");

    let mut msg = String::new();
    loop {
        print!("Enter your message: ");
        io::stdout().flush().unwrap(); // to ignore newline
    
        io::stdin().read_line(&mut msg).expect("Error: Failed to read line");

        socket.send_to(msg.as_bytes(), server_addr.clone()).expect("Failed to send message");

        let mut response = vec![0; 1024];
        match socket.recv_from(&mut response) {
            Ok((bytes_received, _)) => {
                if bytes_received == 0 {
                    println!("Failed to receive from {}", server_addr);
                    return;
                }
                let response_str = String::from_utf8_lossy(&response[..bytes_received]);
                println!("Server response: {}", response_str);
            }
            Err(ref err) => {
                panic!("Failed to receive response: {:?}", err);
            }
        }

        if msg.trim() == "exit" {
            break;
        }
        
        msg.clear();
    }

    println!("Happy terminating from client side");
}
