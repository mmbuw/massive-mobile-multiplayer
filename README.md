# Massive Mobile Multiplayer Framework
The Massive Mobile Multiplayer Framework is a sofware platform designed to enable setup-free interaction with shared public displays through personal mobile devices. In this repository, we provide its backend implementation together with the MMMBall application, a real-time soccer game to which client devices can connect and play along. Using a frontend interface which resembles a game controller, users can control their respective soccer players easily.


# Setup and Installation
 - Install necessary dependencies on server: SFML2, python3; if you use a Raspberry Pi, a special fork of SFML2 (e.g https://github.com/mickelson/sfml) needs to be compiled
 - Install a web server and make its root directory point to the www_root symlink in the project's root directory
 - Connect the server to a network from which it is reachable for client devices, or open a Wifi network yourself (see troubleshooting section for hints)
 - Paste the IP address for accessing the server to the WebSocket creation in frontend/exp_multitouch/js/controller.js (towards the end)
 - Compile the backend module using the provided Makefile in backend/ and MMMBall's configuration file: make CONFIG=configs/mmm-ball
 - Run the compiled backend module in backend/build. The application needs to have read and write access to /dev/uinput in order to create event devices
 - Compile MMMBall using the provided Makefile in game/
 - Run the compiled MMMBall application in game/build. You need to be superuser for that in order to have access to the event devices
 - On the client devices, connect to the web server in a browser, and follow the displayed instructions to join the game

 - For new application contexts to be developed, the stubs in templates/ may help.

# Troubleshooting
## Wireless network and DHCP server
We have used and tested two platforms on which we opened a Wifi network and assigned IP addresses to the connecting client devices. Therefore, we assigned the IP address 29.4.93.1 to the respective server and gave out an address range of 29.4.93.10 to 29.4.93.254 to connecting client devices. 

### Using hostapd and dnsmasq on Raspbian
Example configuration of hostapd (make sure your Wifi adapter supports AP mode):

```
 interface=wlan0
 driver=nl80211
 
 logger_syslog=-1
 logger_syslog_level=0
 logger_stdout=-1
 logger_stdout_level=0
 
 dump_file=/tmp/hostapd.dump
 ctrl_interface=/var/run/hostapd
 ctrl_interface_group=0
 
 ssid=MMM
 country_code=DE
 hw_mode=g
 channel=6
 beacon_int=100
 dtim_period=2
 max_num_sta=32
 rts_threshold=2347
 fragm_threshold=2346
 
 macaddr_acl=0
 ignore_broadcast_ssid=0
 
 eapol_key_index_workaround=0
 eap_server=0
 
 own_ip_addr=127.0.0.1
```

Example configuration of dnsmasq:

```
  address=/#/29.4.93.1
  interface=wlan0
  dhcp-range=29.4.93.2,29.4.93.254,15m
```

### Using network-manager and isc-dhcp-server on Ubuntu
Example configuration of connection in network-manager (make sure your Wifi adapter supports AP mode):

```
[connection]
id=MMM-dragon
uuid=[WIFI NETWORK'S UUID]
type=802-11-wireless
permissions=user:[YOUR USERNAME]:;

[802-11-wireless]
ssid=MMM
mode=ap
mac-address=[YOUR WIFI ADAPTER'S MAC ADDRESS]

[ipv6]
method=auto

[ipv4]
method=manual
address1=29.4.93.1/24,29.4.93.1
```

Example configuration of isc-dhcp-server:

```
ddns-update-style none;
default-lease-time 600;
max-lease-time 7200;

subnet 29.4.93.0 netmask 255.255.255.0 {
  range 29.4.93.10 29.4.93.254
}
```


## Firewall configuration
When MMMBall does not respond to the inputs given by the device, the firewall may be configured to block the incoming inputs. As MMMBall operates on port 53000, try to add the following rules to iptables:

`sudo iptables -A INPUT -p tcp --dport 53000 -j ACCEPT`<br>
`sudo iptables -A OUTPUT -p tcp --dport 53000 -j ACCEPT`

If you want to keep these rules after a reboot, the `iptables-persistent` package helps.

# Contributors
Tim Weißker (tim.weissker@uni-weimar.de)<br>
Andreas Berst (andreas.berst@uni-weimar.de)<br>
Johannes Hartmann (johannes.hartmann@uni-weimar.de)<br>
Florian Echtler (florian.echtler@uni-weimar.de)
