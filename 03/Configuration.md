# Linux

To see the host name
```sh
hostname
```

To change host name open file at `/etc/hostname` and `reboot` the machine
```sh
nano /etc/hostname
```

To see the ip address
```sh
ip -c a
```

To se the route information
```sh
ip -c route
```


### Setup Static IP Address in Linux using Netplan

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
                addresses: [8.8.8.8,8.8.4.4]
```


```sh
sudo netplan try
```

```sh
sudo netplan apply
```
















# Windows

```sh
ifconfig /all
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






# Resources

https://www.youtube.com/watch?v=5Zlkz0KKZyg
https://www.youtube.com/watch?v=AegRx0EzHmU
https://www.youtube.com/watch?v=2Btkx9toufg




