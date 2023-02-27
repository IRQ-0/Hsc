#include "site.h"

std::string createSite(siteParams pra) {
	std::string out =
	std::string("<html> <head> <title> Panel </title> </head> <body> <h1> Panel </h1> <br/> <br/> ") +

    // MOTHERBOARD
    std::string("<h2> Motherboard </h2> <br /> <table border=\"1\"> <tr> <td> Motherboard name </td> <td> ") + pra.motherboardName +
    std::string("</td> </tr> <tr> <td> Motherboard version </td> <td> ") + pra.motherboardVersion +
    std::string("</td> </tr> </table>") +

    // BIOS
    std::string("<h2> BIOS </h2> <br /> <table border=\"1\"> <tr> <td> Bios release date </td> <td> ") + pra.BIOSDate +
    std::string("</td> </tr> <tr> <td> Bios vendor </td> <td> ") + pra.BIOSVendor +
    std::string("</td> </tr> <tr> <td> Bios version </td> <td> ") + pra.BIOSVersion +
    std::string("</td> </tr> </table>") +

   	// CPU
    std::string("<h2> CPU </h2> <br /> <table border=\"1\"> <tr> <td> Cpu clock </td> <td> ") + pra.CPUClock +
    std::string("</td> </tr> <tr> <td> Cpu name </td> <td> ") + pra.CPUName +
    std::string("</td> </tr> <tr> <td> Cpu type </td> <td> ") + pra.CPUType +
    std::string("</td> </tr> <tr> <td> Cpu threads </td> <td> ") + pra.CPUThreads +
	std::string("</td> </tr> </table>") +

    // Memory
    std::string("<h2> Memory </h2> <br /> <table border=\"1\"> <tr> <td> Minimum allocation adress </td> <td> ") + pra.memoryMinAllocAddr +
    std::string("</td> </tr> <tr> <td> Maximum allocation adress </td> <td> ") + pra.memoryMaxAllocAddr +
    std::string("</td> </tr> <tr> <td> Total virtual memory </td> <td> ") + pra.memoryVirtualTotal +
	std::string("</td> </tr> <tr> <td> Total physical memory </td> <td> ") + pra.memoryPhysicalTotal +
    std::string("</td> </tr> </table>") +

    // Inet
    std::string("<h2> Internet </h2> <br /> <table border=\"1\" <tr> <td> Host </td> <td> ") + pra.inetInfo.host +
    std::string("</td> </tr> <tr> <td> Domain </td> <td> ") + pra.inetInfo.domain +
    std::string("</td> </tr> <tr> <td> DNS Servers list </td> <td> ") + pra.inetInfo.dnsServersList +
    std::string("</td> </tr> <tr> <td> Node </td> <td> ") + pra.inetInfo.node +
    std::string("</td> </tr> <tr> <td> DHCP </td> <td> ") + pra.inetInfo.dhcp +
    std::string("</td> </tr> <tr> <td> Routing </td> <td> ");

    if (pra.inetInfo.routing == ENABLED)
        out += std::string("Enabled");
    else
        out += std::string("Disabled");

    out += std::string("</td> </tr> <tr> <td> Arp </td> <td> ");

    if (pra.inetInfo.arp == ENABLED)
        out += std::string("Enabled");
    else
        out += std::string("Disabled");

    out += std::string("</td> </tr> <tr> <td> DNS </td> <td> ");

    if (pra.inetInfo.dns == ENABLED)
        out += std::string("Enabled");
    else
        out += std::string("Disabled");

    out += std::string("</td> </tr> </table>");

    // Button for ipconfig
    out += std::string("<br /> <br /> <a href=\"ipconfig.hsc\"> Get raw ip configuration </a>");

    // Log out
    out += std::string("<br /> <a href=\"index.html\"> Log out </a>");

    std::ifstream t(STATUS_HTML_FILENAME);
    std::stringstream buffer;
    buffer << t.rdbuf();

    out += buffer.str();

    return out;
}