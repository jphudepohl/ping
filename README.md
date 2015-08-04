# ndnping and ndnpingserver

## Credits
The majority of this code is not my own. Please see https://github.com/named-data/ndn-tools/tree/master/tools/ping for the original code and install instructions. I added a command line option to make and sign data packets in advance. This can be accessed by running the ndnpingserver with the `-a` flag. For example::
    
    ndnpingserver -a /ping

## Introduction

**ndnping** and **ndnpingserver** are reachability testing tools for
[Named Data Networking](http://named-data.net). They test the reachability between two nodes. The
client sends an Interest intended for a node running **ndnpingserver**. The ping server then sends
Data in response. The client then calculates the roundtrip time for the Interest-Data exchange, or
marks it as a timeout if the Data is not received within the timeout period.

## Using the Client

The client can be invoked by calling **ndnping** with a name to ping. For example, to ping
`ndn:/edu/arizona`, one would execute::

    ndnping ndn:/edu/arizona

There are also a variety of options to control the behavior of the ping client. For example, to
send only four pings to `ndn:/edu/arizona`, displaying a timestamp with each received Data or
timeout, type::

    ndnping -c 4 -t ndn:/edu/arizona

A list of the available options can be found with `man ndnping`.

## Using the Server

The server can be invoked by calling **ndnpingserver** with a name to listen for pings to. For
example, to listen for pings to `ndn:/edu/arizona`, one would execute::

    ndnpingserver ndn:/edu/arizona

There are also a variety of options to control the behavior of the ping server. For example, to
satisfy only 4 ping requests before exiting, execute the following::

    ndnpingserver -p 4 ndn:/edu/arizona

A list of the available options can be found with `man ndnpingserver`.
