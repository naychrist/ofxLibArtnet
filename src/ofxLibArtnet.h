#pragma once

#import "artnet.h"
#import "packets.h"

/*
 @todo:
 - functions to push back vals into DMX data before sending
 - test send/receive with hardware or OLA running on separate machine
*/

namespace ofxLibArtnet
{    

#pragma Mark - Universe class
    
class Universe
{
public:
    
    Universe(int port_id, uint8_t port_addr, int16_t length, uint8_t *data) :
    port_id(port_id),
    port_addr(port_addr),
    length(length),
    data(nullptr)
    {}
    
    int         port_id;    // our id for uni port
    uint8_t     port_addr;  // uni port
    int16_t     length;     // length of DMX data array
    uint8_t     *data;      // DMX data array
    
};
    
#pragma Mark - Node class
    
class Node
{
public:
    
    vector<Universe>    unis;
    
    void setup(int _num_unis = 1, char *_ip_addr = nullptr, uint8_t _subnet_addr = 0)
    {
        ip_addr = _ip_addr;
        subnet_addr = _subnet_addr;
        
        artnetNode  = artnet_new(ip_addr, 1);
        
        if (!artnetNode) {
            printf("Error: %s\n", artnet_strerror());
            std::exit(-1);
        }
        
        artnet_set_long_name(artnetNode, "Art-Net Test");
        artnet_set_short_name(artnetNode, "ANT");
        
        // set the upper 4 bits of the universe address 
        artnet_set_subnet_addr(artnetNode, subnet_addr) ;
        
        //create universes...
        for (int i=0; i<_num_unis; i++) {
            // create the universe
            Universe uni(i,i+1,0,nullptr);
            // enable port        
            artnet_set_port_type(artnetNode, uni.port_id, ARTNET_ENABLE_OUTPUT, ARTNET_PORT_DMX);
            // bind port to universe
            artnet_set_port_addr(artnetNode, uni.port_id, ARTNET_OUTPUT_PORT, uni.port_addr); 
            unis.push_back(uni);
        }
        
        artnet_dump_config(artnetNode);
        
        //set receiver callback
        artnet_set_handler(artnetNode, ARTNET_RECV_HANDLER, artnetReceiverWrapper, this); 
        
        if (artnet_start(artnetNode) != 0) {
            printf("Error: %s\n", artnet_strerror());
            std::exit(-1);
        }  
        
    }
    
    void send()
    {
        
        //artnet_send_poll(artnetNode, nullptr, ARTNET_TTM_DEFAULT);//to test receiving...
        //printf("arnet_get_sd() => %i\n", artnet_get_sd(artnetNode));
        //printf("artnet_read() => %i\n", artnet_read(artnetNode, 1));//works but slows down thread
        
        //this is where we send our DMX...
        for (int i=0; i<unis.size(); i++) {
           artnet_send_dmx(artnetNode, unis[i].port_id, unis[i].length, unis[i].data); 
        }
    }
    
    void close()
    {
        // deallocate memory
        artnet_stop(artnetNode);
        artnet_destroy(artnetNode);
    }
    
#pragma Mark - Callback for receiving
    
    static int artnetReceiverWrapper(artnet_node node, void *pp, void *d) {
        Node* n = (Node*) d;
        return n->artnetReceiver(node, pp);
    }
    
    int artnetReceiver(artnet_node node, void *pp) {
        printf("Receiving Art-Net data!");
        artnet_packet pack = (artnet_packet) pp;
        printf("Received packet sequence %d\n", pack->data.admx.sequence);
        printf("Received packet type %d\n", pack->type);
        printf("Received packet data %s\n", pack->data.admx.data);
        return 0;
    }
    
private:
    
    artnet_node     artnetNode;
    char            *ip_addr;
    uint8_t         subnet_addr; 
    
};

    
}//namespace ofxLibArtnet
  

