#pragma once

#include "ofMain.h"
#include "artnet.h"
#include "packets.h"

/*
 @todo:
 - methods to update data via universe or port ID
 - methods for greater control over specifying universe and port IDs
 - test receive form single device
 - set multiple IPs by joining nodes to enable receiving from multiple devices
*/

namespace ofxLibArtnet
{    

#pragma Mark - Universe class
    
class Universe
{
public:
    
    Universe(int port_id, uint8_t port_addr, int16_t length = 0, uint8_t *data = NULL) :
    port_id(port_id),
    port_addr(port_addr),
    length(length),
    data(data),
    updated(false)
    {}
    
    int         port_id;    // our id for uni port
    uint8_t     port_addr;  // uni port
    int16_t     length;     // length of DMX data array
    uint8_t     *data;      // DMX data array
    bool        updated;
    
};
    
#pragma Mark - Node class
    
class Node
{
public:
    
    void    addUniverses(int _num);
    void    setup(string _ip_addr, bool _sendRaw = false, uint8_t _subnet_addr = 0);
    
    void    updateData(unsigned char * _data, int _length);
    void    updateDataByIndex(int _index, unsigned char * _data, int _length);
    
    int     getNumUniverses();
    
    void    send();
    
    void    close();
    
    
#pragma Mark - Callback for receiving
    //should only work as is if not raw...
    
    int artnetReceiver(artnet_node node, void *pp);
    
    static int artnetReceiverWrapper(artnet_node node, void *pp, void *d) {
        Node* n = (Node*) d;
        return n->artnetReceiver(node, pp);
    };
    
    
private:
    
    artnet_node         artnetNode;
    string              ip_addr;
    uint8_t             subnet_addr; 
    bool                sendRaw;
    vector<Universe>    unis;
    
};

    
}//namespace ofxLibArtnet

