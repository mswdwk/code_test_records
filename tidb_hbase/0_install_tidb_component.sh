#!/bin/bash
tiup list pd

tiup install pd:v6.6.0
tiup install tikv:v6.6.0
echo "prepare install tikv ctl"
tiup install ctl:v6.6.0
