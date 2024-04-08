<div align="center"><b>Configure your PC for LAN in Windows and Linux environment (configuration for Ethernet card)</b></div>

<h2 align="center">Linux Configuration</h2>


### Basic Old Commands

To see all the network configuration
```sh
ifconfig
```

To see the host name
```sh
hostname
```

To change host name open file at `/etc/hostname` and `reboot` the machine
```sh
nano /etc/hostname
```

To Change IP address
```sh
sudo ifconfig eth0 <IP> netmask <Mask_IP>
```


### Permanently Network Configuration


#### Debian

##### Set Static IP Address

Open the following file
```sh
sudo nano /etc/network/interfaces
```


and change accordingly 
```
auto eth0
iface eth0 inet static
  address 192.168.0.2
  netmask 255.255.255.0
  gateway 172.19.50.100
  dns-nameservers 8.8.8.8 8.8.4.4
```


then restart the system to see changes
```sh
sudo systemctl restart networking
```


##### Set Permanent Route

Open the following file
```sh
sudo nano /etc/network/interfaces
```

and change things accordingly
```
auto eth0
iface eth0 inet static
  address 172.19.50.2
  netmask 255.255.255.0
  gateway 172.19.50.100

#########{Static Route}###########
up ip route add 172.19.1.0/24 via 172.19.1.2 dev eth0
```

then restart the system to see changes
```sh
sudo systemctl restart networking
```

####  RHEL

##### Set Static IP Address


Open the following file
```sh
sudo nano /etc/sysconfig/network-scripts/ifcfg-eth0
```


and change accordingly 
```
TYPE=Enternet
PROXY_METHOD=none
BROWSER_ONLY=no
BOOTTPROTO=none
IPADDR=194.168.0.102
PREFIX=24
NETMASK=255.255.255.0
GATEWAY=192.168.0.1
DNS1=8.8.8.8
DEFROUTE=yes
IPV4_FAILURE_FETAL=no
IPV6INIT=yes
IPV6_AUTOCONF=yes
IPV6_DEFROUTE=yes
IPV6_FAILURE_FATAL=no
IPV6_ADDR_GEN_MODE=stable-privacy
NAME="Ethernet connection 1"
UUID=fasd43-dsaf3-dfsa-dsfa-dfg3asf
ONBOOT=yes
```


then restart the system to see changes
```sh
sudo systemctl restart networking
```


##### Set Permanent Route

Open the following file
```sh
sudo nano /etc/sysconfig/network-scripts/route-eth0
```

and change things accordingly
```
172.19.1.0/24 via 172.19.1.2 dev eth0
```

then restart the system to see changes
```sh
sudo systemctl restart networking
```











### Network Configuration using [`ip`](https://www.linode.com/docs/guides/how-to-use-the-linux-ip-command/) command


#### IP Address

To see the ip addresses
```sh
ip addr
```
or `ip a`


To see information about a specific ip address
```sh
ip a show <ip_name>
```
example: `ip a show dev eth0`


To see the ip addresses of IPv4
```sh
ip -4 a
```

To see the ip addresses of IPv6
```sh
ip -6 a
```


To see the ip addresses using color
```sh
ip -c a
```


To see the ip addresses in smart one liner
```sh
ip -br -c a
```

To see the ip addresses in json format
```sh
ip -j a
```

To see the ip addresses in readable json format
```sh
ip -p -j a
```

To see the ip addresses with extra statistics
```sh
ip -s a
```
for more `ip -s -s a`


To add a temporary IP address
```sh
sudo ip a add <ip> <ip_name>
```
example: `sudo ip a add 127.255.255.255/16 dev eth7`


To delete a  IP address
```sh
sudo ip a del <ip> <ip_name>
```
example: `sudo ip a del 127.255.255.255/16 dev eth7`


To configure the address as a broadcast address using the keyword `brd`
```sh
sudo ip a add 172.31.255.255 brd + dev eth1
```



#### Link IP


To view information about all network interfaces on the server
```sh
ip link show
```
or `ip l`


To see the ip addresses in smart one liner
```sh
ip -br -c l
```


To disable the ip link
```sh
sudo ip link set <ip_name> down
```
example: `sudo ip link set eth1 down`


To enable the ip link
```sh
sudo ip link set <ip_name> up
```


To set the maximum transmission unit (MTU) size for the link
```sh
sudo ip link set mtu 1600 <ip_name>
```


#### IP Route

To view all routes installed in the routing database
```sh
ip route show
```
or `ip r`




### Modern Ubuntu Network Configuration using `netplan`

```sh
sudo nano /etc/netplan/01-network-manager.yaml
```

```yaml
network:
  version: 2
  renderer: NetworkManager
```

convert this to bellow file

```yaml
network:
  version: 2
  renderer: NetworkManager
  ethernets:
    enp0s3:
      dhcp4: no
      addresses: [192.168.1.4/24]
      gateway4: 192.1.1.1
      nameserver:
        addresses: [8.8.8.8, 8.8.4.4]
```

```sh
sudo netplan try
```

```sh
sudo netplan apply
```







<h2 align="center">Windows Configuration</h2>

To see all the network configuration
```sh
ipconfig /all
```

To see DNS records

```sh
nslookup <IP>
```

To check connection

```sh
ping <IP>
```

To check hop in network

```sh
tracert <IP>
```

To desplay the network statistics of our local computer

```sh
netstat
```

To see all ip along with the MAC address

```sh
arp -a
```

#### Configure IP address, Gateway and DNS

Run `cmd` or `pwsh` as Admin

To see only the network interface

```sh
netsh interface show interface
```

To see the network configuration using `netsh`
```sh
netsh interface ip show config
```


To configure a static IP of local machine
```sh
netsh interface ip set address name="<Network_Name>" static <IP> <Mask> <Gateway>
```
Example: `netsh interface ip set address name="Enternet" static 192.168.1.50 255.255.255.0 192.168.1.1`


To configure a dynamic IP of local machine
```sh
netsh interface ip set address name="<Network_Name>" dhcp
```


To configure static IP of DNS
```sh
netsh interface ip set dns "<Network_Name>" static <IP>
```

To configure dynamic IP of DNS
```sh
netsh interface ip set dns "<Network_Name>" dhcp
```

To configure IP of secondary DNS
```sh
netsh interface ip add dns name="<Network_Name>" 208.67.220.220 index=2
```



## References

[More about ip command in Linux](https://www.cyberciti.biz/faq/linux-ip-command-examples-usage-syntax/)
[Ubuntu Server: Getting started with a Linux Server](https://www.youtube.com/watch?v=2Btkx9toufg&ab_channel=ByteMyPi)
[Ubuntu 22.04 : Change IP Configuration](https://www.youtube.com/watch?v=5Zlkz0KKZyg&ab_channel=NelisysDeveloper)
[Top 20 Windows Network Commands](https://www.youtube.com/watch?v=AegRx0EzHmU&ab_channel=SheshChauhanITTrainer)
