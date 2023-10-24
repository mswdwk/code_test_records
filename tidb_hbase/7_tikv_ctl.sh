#!/bin/bash
#tikv-ctl --ca-path ca.pem --cert-path client.pem --key-path client-key.pem --host 127.0.0.1:20160 <subcommands>
#tikv-ctl --host 127.0.0.1:20160 help 
#tiup ctl:v6.6.0 tikv --host 127.0.0.1:20160 help 
#tiup ctl:v6.6.0 tikv --host 127.0.0.1:20160  region-properties -r 8
#tiup ctl:v6.6.0 tikv --host 127.0.0.1:20160 -c'clumn family' -f 7480000000000000615f728000000000000001





tiup ctl:v6.6.0 tikv --host 127.0.0.1:20160 $* 


