#include "ofxLibArtnet.h"

namespace ofxLibArtnet {
     
void Node::addUniverses(int _num) {
    
    if (unis.size()) {
        cout<<"Error: can not call addUniverses() more than once"<<endl;
        return;
    }
    
    for (int i=0; i<_num; i++) {
        unis.push_back( Universe(i,i) );
    }
    
}

void Node::setup(string _ip_addr, bool _sendRaw, uint8_t _subnet_addr)
{
    ip_addr = _ip_addr;
    subnet_addr = _subnet_addr;
    sendRaw = _sendRaw;
    
    artnetNode  = artnet_new(ip_addr.c_str(), 0);//int = verbose
    //@todo: allow for multiple nodes then artnet_join(artnetNode1, artnetNodeN)
    
    if (!artnetNode) {
        printf("Error: %s\n", artnet_strerror());
        std::exit(-1);
    }
    
    artnet_set_long_name(artnetNode, "ofxLibArtnet");
    artnet_set_short_name(artnetNode, "OF");
    if (sendRaw) artnet_set_node_type(artnetNode, ARTNET_RAW);
    
    // set the upper 4 bits of the universe address 
    artnet_set_subnet_addr(artnetNode, subnet_addr) ;
    
    if (!unis.size()) {
        addUniverses(1);
        cout<<"Creating single universe with port and ID 0."<<endl;
    }
    
    for (int i=0; i<unis.size(); i++) {
        // enable port        
        artnet_set_port_type(artnetNode, unis[i].port_id, ARTNET_ENABLE_OUTPUT, ARTNET_PORT_DMX);
        // bind port to universe
        artnet_set_port_addr(artnetNode, unis[i].port_id, ARTNET_OUTPUT_PORT, unis[i].port_addr); 
    }
    
    artnet_dump_config(artnetNode);
    
    //set receiver callback
    artnet_set_handler(artnetNode, ARTNET_RECV_HANDLER, artnetReceiverWrapper, this); 
    
    if (artnet_start(artnetNode) != 0) {
        printf("Error: %s\n", artnet_strerror());
        std::exit(-1);
    }  
    
};

void Node::updateData(unsigned char * _data, int _length) {
    unis[0].data = _data;
    unis[0].length = _length;
    unis[0].updated = true;
};

void Node::updateDataByIndex(int _index, unsigned char * _data, int _length) {
    if (_index < unis.size()) {
        unis[_index].data = _data;
        unis[_index].length = _length;
        unis[_index].updated = true;
    }
};

int Node::getNumUniverses() {
    return unis.size();
};

void Node::send()
{
    
    //artnet_send_poll(artnetNode, NULL, ARTNET_TTM_DEFAULT);//to test receiving...
    //printf("arnet_get_sd() => %i\n", artnet_get_sd(artnetNode));
    //printf("artnet_read() => %i\n", artnet_read(artnetNode, 1));//works but slows down thread
    
    for (int i=0; i<unis.size(); i++) {
        if (unis[i].updated) {
            if (!sendRaw) {
                artnet_send_dmx(artnetNode, unis[i].port_id, unis[i].length, unis[i].data);  
            } else {
                artnet_raw_send_dmx(artnetNode, unis[i].port_addr, unis[i].length, unis[i].data);   
            }
            unis[i].updated = false;
        }
    }
};

void Node::close()
{
    // deallocate memory
    artnet_stop(artnetNode);
    artnet_destroy(artnetNode);
};

#pragma Mark - Callback for receiving
//should only work as is if not sending raw...

int Node::artnetReceiver(artnet_node node, void *pp) {
    printf("Receiving Art-Net data!");
    artnet_packet pack = (artnet_packet) pp;
    printf("Received packet sequence %d\n", pack->data.admx.sequence);
    printf("Received packet type %d\n", pack->type);
    printf("Received packet data %s\n", pack->data.admx.data);
    return 0;
};

}//namespace ofxLibArtnet



