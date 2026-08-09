#!/bin/sh

set -x

echo "hostapd.sh: " $1

bash /usr/bin/hostapd_genconf.sh

func_start_wl(){
	start-stop-daemon -S -m -p /var/run/hostapd.wlan1.pid -x /usr/sbin/hostapd -- -t /var/run/hostapd/hostapd-wlan1.conf > /tmp/hostapd-wlan1.log 2>&1 &
}
func_start_rt(){
	start-stop-daemon -S -m -p /var/run/hostapd.wlan0.pid -x /usr/sbin/hostapd -- -t /var/run/hostapd/hostapd-wlan0.conf > /tmp/hostapd-wlan0.log 2>&1 &
}

func_stop_rt(){

        start-stop-daemon -K \
             -p /var/run/hostapd.wlan0.pid \
             -x /usr/sbin/hostapd
	sleep 1
}

func_stop_wl(){
        start-stop-daemon -K \
             -p /var/run/hostapd.wlan1.pid \
             -x /usr/sbin/hostapd
	sleep 1

}

case "$1" in
start_rt)
    func_start_rt
    ;;
start_wl)
    func_start_wl
    ;;
stop_wl)
    func_stop_wl
    ;;
stop_rt)
    func_stop_rt
    ;;
restart_rt)
    func_stop_rt
    func_start_rt
    ;;
restart_wl)
    func_stop_wl
    func_start_wl
    ;;
*)
    echo "Usage: $0 { start | stop | restart }"
    exit 1
    ;;
esac
