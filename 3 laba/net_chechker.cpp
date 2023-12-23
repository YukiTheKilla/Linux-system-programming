#include <iostream> 
#include <pcap.h> 
#include <netinet/in.h> 
#include <netinet/if_ether.h> 
 
void packetHandler(unsigned char* user, const struct pcap_pkthdr* pkthdr, const unsigned char* packet) { 
    std::cout << "Ethernet Frame Captured. Length: " << pkthdr->len << " bytes" << std::endl; 
 
    ether_header* ethHeader = (ether_header*)packet; 
    std::cout << "Source MAC: "; 
    for (int i = 0; i < 6; ++i) { 
        printf("%02x", ethHeader->ether_shost[i]); 
        if (i < 5) std::cout << ":"; 
    } 
    std::cout << std::endl; 
 
    std::cout << "Destination MAC: "; 
    for (int i = 0; i < 6; ++i) { 
        printf("%02x", ethHeader->ether_dhost[i]); 
        if (i < 5) std::cout << ":"; 
    } 
    std::cout << std::endl; 
 
} 
 
int main() { 
    char errbuf[PCAP_ERRBUF_SIZE]; 
 
    const char* dev = "eth0"; 
 
    pcap_t* handle = pcap_open_live(dev, BUFSIZ, 1, 1000, errbuf); 
 
    if (handle == nullptr) { 
        std::cerr << "Could not open device " << dev << ": " << errbuf << std::endl; 
        return 1; 
    } 
 
    std::cout << "Packet capture on device " << dev << " started..." << std::endl; 
 
    // Start capturing packets 
    pcap_loop(handle, 0, packetHandler, nullptr); 
 
    pcap_close(handle); 
 
    return 0; 
}
