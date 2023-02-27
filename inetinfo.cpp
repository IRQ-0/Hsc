#include "inetinfo.h"

#define BUFFER_SIZE 4096

INET_INFO getInetInfo(void) {
	FIXED_INFO *interInfo;
	INET_INFO out;
    ULONG len;
    DWORD ret;
    IP_ADDR_STRING *ipAddr;

	interInfo = (FIXED_INFO*) malloc(sizeof(FIXED_INFO));
    len = sizeof(FIXED_INFO);
	
	if (GetNetworkParams(interInfo, &len) == NO_ERROR) {
        out.host = std::string(interInfo->HostName);
        out.domain = std::string(interInfo->DomainName);

        out.dnsServersList = std::string(interInfo->DnsServerList.IpAddress.String);

        /*ipAddr = interInfo->DnsServerList.Next;
        while (ipAddr) {
            printf("\t%s\n", ipAddr->IpAddress.String);
            ipAddr = ipAddr->Next;
        }*/

        switch (interInfo->NodeType) {
	        case BROADCAST_NODETYPE:
    	        out.node = std::string("Broadcast node");
            break;
    	    case PEER_TO_PEER_NODETYPE:
    	        out.node = std::string("Peer to Peer node");
            break;
    	    case MIXED_NODETYPE:
    	        out.node = std::string("Mixed node");
   	        break;
    	    case HYBRID_NODETYPE:
        	    out.node = std::string("Hybrid node");
       	    break;
        	default:
            	out.node = std::string("Unknown node type");
        }

        out.dhcp = std::string(interInfo->ScopeId);

        if (interInfo->EnableRouting)
            out.routing = ENABLED;
        else
            out.routing = DISABLED;

        if (interInfo->EnableProxy)
            out.arp = ENABLED;
        else
            out.arp = DISABLED;

        if (interInfo->EnableDns)
            out.dns = ENABLED;
        else
            out.dns = DISABLED;
		
    }
	
    free(interInfo);


    return out;
}

inline std::string getParamName(std::string line, int* ifDns) {
    std::string::size_type nend, pend;
    std::string out;
    const std::string dnsServersStr = "DNS Servers";
    nend = line.find(":");
    
    std::string possibleDns = (line.substr(3, nend - 3)).substr(0, dnsServersStr.length());
    
        
    if (possibleDns.compare(dnsServersStr) == 0) {
        *ifDns = 1;
    }
    
    std::string paramName = std::string("<tr> <td> ") + line.substr(3, nend - 3) + std::string("</td>");
    
    #ifdef NO_DOTS
            
        for (int i = 0; i < paramName.length(); i++) {
            if (paramName[i] == '.')
                paramName[i] = ' ';
        }
    
    #endif
    
    out += paramName;
    out += std::string("<td> ") + line.substr(nend + 1) + std::string("</td> </tr>");
    
    return out;
}


inline std::string dns(std::string line, int* ifDns) {
    std::string::size_type nend;
    std::string out = std::string("<tr> <td> </td> <td> ");
    
    if (line[4] != ' ') {
        *ifDns = 0;
        return getParamName(line, ifDns);
    }
    
    int i;
    for (i = 0; i < line.length(); i++) {
        if (line[i] != ' ') { break; }
    }
    
    out += line.substr(i);
    out += std::string(" </td> </tr>");
    
    return out;
}


std::string getIpconfig(void) {
    std::string raw = getRaw();
    int tabDef = 0, ifDns = 0;
    std::istringstream f(raw);
    std::string line, out;
    
    out = std::string("<html> <head> <title> Raw ipconfig info </title> </head> <body> <div style=\"font-family: consolas;\">");
    
    while (std::getline(f, line)) {
        if (line[0] != ' ' && line.length() != 0) {
            if (tabDef == 1) {
                out += std::string("</table>");
            }

            out += std::string("<h2> ") + line + std::string("</h2> <br />");
            out += std::string("<table border=\"1\">");
        } else if (line.length() != 0) {
            tabDef = 1;
            if (ifDns == 0) {
                out += getParamName(line, &ifDns);
            } else if (ifDns == 1) {
                out += dns(line, &ifDns);
            }
        }
    }
    
    out += std::string("</table> </div> <br /> <br /> <a href=\"http://127.0.0.1:8000/panel.hsc\"> Go back </a>  </body> </html>");
    
    return out;
}

std::string getRaw(void) {
    const std::string command = IPCONFIG_COMMAND;

    std::array<char, BUFFER_SIZE> buffer;
    std::string result;
    std::unique_ptr<FILE, decltype(&pclose)> pipe(popen(command.c_str(), "r"), pclose);

    if (!pipe)
        throw std::runtime_error("popen() failed!");

    while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr)
        result += buffer.data();
    
    return result;
}