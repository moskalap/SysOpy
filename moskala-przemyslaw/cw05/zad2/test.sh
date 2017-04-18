#!/bin/bash


gnome-terminal -x sh -c "./master potok 600; rm potok; rm data; rm nohup.out; bash" 


nohup ./slave potok 99999 100 &
nohup ./slave potok 99999 100 &
nohup ./slave potok 99999 100 &
nohup ./slave potok 99999 100 &
nohup ./slave potok 99999 100 &
nohup ./slave potok 99999 100 &
nohup ./slave potok 99999 100 &
nohup ./slave potok 99999 100 &
nohup ./slave potok 99999 100 &
nohup ./slave potok 99999 100 &

