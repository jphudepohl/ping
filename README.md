# ndnping and ndnpingserver

## Credits
The majority of this code is not my own. Please see https://github.com/named-data/ndn-tools/tree/master/tools/ping for the original code and install instructions.

## Modification
I added a command line option to make and sign data packets in advance. Signing data packets often takes a signicant amount of time (~4 ms), so doing this in advance allows ndnping to produce a more accurate round trip time (RTT) result. My modification uses the first interest packet received to identify the interest name and ping sequence number, so the initial RTT should be ignored. 

To use this feature, run the ndnpingserver with the `-a` flag followed by `[number of data packets to make and sign in advance]`. For example, to make and sign 100 data packets in advance and to advertise the prefix `/ping`, run:
    
    ndnpingserver -a 100 /ping

