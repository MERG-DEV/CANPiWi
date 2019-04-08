#include "opc_codes.h"

opc_container::opc_container(){
    this->populate();
}

opc_code* opc_container::getByCode(int code){
    opc_code *p_opc_code;
    if (opcs_by_code.find(code) != opcs_by_code.end()){
        p_opc_code = opcs_by_code[code];
        return p_opc_code;
    }
    return NULL;
}
opc_code* opc_container::getByName(string name){
    opc_code *p_opc_code;
    if (opcs_by_name.find(name) != opcs_by_name.end()){
        p_opc_code = opcs_by_name[name];
        return p_opc_code;
    }
    return NULL;
}


void opc_container::populate_both(int code, string name, string description){
    opcs_by_code.insert(pair<int, opc_code*>(code, new opc_code(code, name, description)));
    opcs_by_name.insert(pair<string, opc_code*>(name, new opc_code(code, name, description)));
}

void opc_container::populate(){
    populate_both(OPC_ACK, "OPC_ACK", "General ack");
    populate_both(OPC_NAK, "OPC_NAK", "General nak");
    populate_both(OPC_HLT, "OPC_HLT", "Bus Halt");
    populate_both(OPC_BON, "OPC_BON", "Bus on");
    populate_both(OPC_TOF, "OPC_TOF", "Track off");
    populate_both(OPC_TON, "OPC_TON", "Track on");
    populate_both(OPC_ESTOP, "OPC_ESTOP", "Track stopped");
    populate_both(OPC_ARST, "OPC_ARST", "System reset");
    populate_both(OPC_RTOF, "OPC_RTOF", "Request track off");
    populate_both(OPC_RTON, "OPC_RTON", "Request track on");
    populate_both(OPC_RESTP, "OPC_RESTP", "Request emergency stop all");
    populate_both(OPC_RSTAT, "OPC_RSTAT", "Request command station status");
    populate_both(OPC_QNN, "OPC_QNN", "Query nodes");
    populate_both(OPC_RQNP, "OPC_RQNP", "Read node parameters");
    populate_both(OPC_RQMN, "OPC_RQMN", "Request name of module type");
    populate_both(OPC_KLOC, "OPC_KLOC", "Release engine by handle");
    populate_both(OPC_QLOC, "OPC_QLOC", "Query engine by handle");
    populate_both(OPC_DKEEP, "OPC_DKEEP", "Keep alive for cab");
    populate_both(OPC_DBG1, "OPC_DBG1", "Debug message with 1 status byte");
    populate_both(OPC_EXTC, "OPC_EXTC", "Extended opcode");
    populate_both(OPC_RLOC, "OPC_RLOC", "Request session for loco");
    populate_both(OPC_QCON, "OPC_QCON", "Query consist");
    populate_both(OPC_SNN, "OPC_SNN", "Set node number");
    populate_both(OPC_STMOD, "OPC_STMOD", "Set Throttle mode");
    populate_both(OPC_PCON, "OPC_PCON", "Consist loco");
    populate_both(OPC_KCON, "OPC_KCON", "De-consist loco");
    populate_both(OPC_DSPD, "OPC_DSPD", "Loco speed/dir");
    populate_both(OPC_DFLG, "OPC_DFLG", "Set engine flags");
    populate_both(OPC_DFNON, "OPC_DFNON", "Loco function on");
    populate_both(OPC_DFNOF, "OPC_DFNOF", "Loco function off");
    populate_both(OPC_SSTAT, "OPC_SSTAT", "Service mode status");
    populate_both(OPC_RQNN, "OPC_RQNN", "Request Node number in setup mode");
    populate_both(OPC_NNREL, "OPC_NNREL", "Node number release");
    populate_both(OPC_NNACK, "OPC_NNACK", "Node number acknowledge");
    populate_both(OPC_NNLRN, "OPC_NNLRN", "Set learn mode");
    populate_both(OPC_NNULN, "OPC_NNULN", "Release learn mode");
    populate_both(OPC_NNCLR, "OPC_NNCLR", "Clear all events");
    populate_both(OPC_NNEVN, "OPC_NNEVN", "Read available event slots");
    populate_both(OPC_NERD, "OPC_NERD", "Read all stored events");
    populate_both(OPC_RQEVN, "OPC_RQEVN", "Read number of stored events");
    populate_both(OPC_WRACK, "OPC_WRACK", "Write acknowledge");
    populate_both(OPC_RQDAT, "OPC_RQDAT", "Request node data event");
    populate_both(OPC_RQDDS, "OPC_RQDDS", "Request short data frame");
    populate_both(OPC_BOOT, "OPC_BOOT", "Put node into boot mode");
    populate_both(OPC_ENUM, "OPC_ENUM", "Force self enumeration for CAN_ID");
    populate_both(OPC_RST, "OPC_RST", "Reset node");
    populate_both(OPC_EXTC1, "OPC_EXTC1", "Extended opcode with 1 data byte");
    populate_both(OPC_DFUN, "OPC_DFUN", "Set engine functions");
    populate_both(OPC_GLOC, "OPC_GLOC", "Get loco (with support for steal/share)");
    populate_both(OPC_ERR, "OPC_ERR", "Command station error");
    populate_both(OPC_CMDERR, "OPC_CMDERR", "Errors from nodes during config");
    populate_both(OPC_EVNLF, "OPC_EVNLF", "Event slots left response");
    populate_both(OPC_NVRD, "OPC_NVRD", "Request read of node variable");
    populate_both(OPC_NENRD, "OPC_NENRD", "Request read stored event by index");
    populate_both(OPC_RQNPN, "OPC_RQNPN", "Request read module parameters");
    populate_both(OPC_NUMEV, "OPC_NUMEV", "Number of events stored response");
    populate_both(OPC_CANID, "OPC_CANID", "Set specified CANID.");
    populate_both(OPC_EXTC2, "OPC_EXTC2", "Extended opcode with 2 data bytes");
    populate_both(OPC_RDCC3, "OPC_RDCC3", "3 byte DCC packet");
    populate_both(OPC_WCVO, "OPC_WCVO", "Write CV byte Ops mode by handle");
    populate_both(OPC_WCVB, "OPC_WCVB", "Write CV bit Ops mode by handle");
    populate_both(OPC_QCVS, "OPC_QCVS", "Read CV");
    populate_both(OPC_PCVS, "OPC_PCVS", "Report CV");
    populate_both(OPC_ACON, "OPC_ACON", "on event");
    populate_both(OPC_ACOF, "OPC_ACOF", "off event");
    populate_both(OPC_AREQ, "OPC_AREQ", "Accessory Request event");
    populate_both(OPC_ARON, "OPC_ARON", "Accessory response event on");
    populate_both(OPC_AROF, "OPC_AROF", "Accessory response event off");
    populate_both(OPC_EVULN, "OPC_EVULN", "Unlearn event");
    populate_both(OPC_NVSET, "OPC_NVSET", "Set a node variable");
    populate_both(OPC_NVANS, "OPC_NVANS", "Node variable value response");
    populate_both(OPC_ASON, "OPC_ASON", "Short event on");
    populate_both(OPC_ASOF, "OPC_ASOF", "Short event off");
    populate_both(OPC_ASRQ, "OPC_ASRQ", "Short Request event");
    populate_both(OPC_PARAN, "OPC_PARAN", "Single node parameter response");
    populate_both(OPC_REVAL, "OPC_REVAL", "Request read of event variable");
    populate_both(OPC_ARSON, "OPC_ARSON", "Accessory short response on event");
    populate_both(OPC_ARSOF, "OPC_ARSOF", "Accessory short response off event");
    populate_both(OPC_EXTC3, "OPC_EXTC3", "Extended opcode with 3 data bytes");
    populate_both(OPC_RDCC4, "OPC_RDCC4", "4 byte DCC packet");
    populate_both(OPC_WCVS, "OPC_WCVS", "Write CV service mode");
    populate_both(OPC_ACON1, "OPC_ACON1", "On event with one data byte");
    populate_both(OPC_ACOF1, "OPC_ACOF1", "Off event with one data byte");
    populate_both(OPC_REQEV, "OPC_REQEV", "Read event variable in learn mode");
    populate_both(OPC_ARON1, "OPC_ARON1", "Accessory on response (1 data byte)");
    populate_both(OPC_AROF1, "OPC_AROF1", "Accessory off response (1 data byte)");
    populate_both(OPC_NEVAL, "OPC_NEVAL", "Event variable by index read response");
    populate_both(OPC_PNN, "OPC_PNN", "Response to QNN");
    populate_both(OPC_ASON1, "OPC_ASON1", "Accessory short on with 1 data byte");
    populate_both(OPC_ASOF1, "OPC_ASOF1", "Accessory short off with 1 data byte");
    populate_both(OPC_ARSON1, "OPC_ARSON1", "Short response event on with one data byte");
    populate_both(OPC_ARSOF1, "OPC_ARSOF1", "Short response event off with one data byte");
    populate_both(OPC_EXTC4, "OPC_EXTC4", "Extended opcode with 4 data bytes");
    populate_both(OPC_RDCC5, "OPC_RDCC5", "5 byte DCC packet");
    populate_both(OPC_WCVOA, "OPC_WCVOA", "Write CV ops mode by address");
    populate_both(OPC_FCLK, "OPC_FCLK", "Fast clock");
    populate_both(OPC_ACON2, "OPC_ACON2", "On event with two data bytes");
    populate_both(OPC_ACOF2, "OPC_ACOF2", "Off event with two data bytes");
    populate_both(OPC_EVLRN, "OPC_EVLRN", "Teach event");
    populate_both(OPC_EVANS, "OPC_EVANS", "Event variable read response in learn mode");
    populate_both(OPC_ARON2, "OPC_ARON2", "Accessory on response");
    populate_both(OPC_AROF2, "OPC_AROF2", "Accessory off response");
    populate_both(OPC_ASON2, "OPC_ASON2", "Accessory short on with 2 data bytes");
    populate_both(OPC_ASOF2, "OPC_ASOF2", "Accessory short off with 2 data bytes");
    populate_both(OPC_ARSON2, "OPC_ARSON2", "Short response event on with two data bytes");
    populate_both(OPC_ARSOF2, "OPC_ARSOF2", "Short response event off with two data bytes");
    populate_both(OPC_EXTC5, "OPC_EXTC5", "Extended opcode with 5 data bytes");
    populate_both(OPC_RDCC6, "OPC_RDCC6", "6 byte DCC packets");
    populate_both(OPC_PLOC, "OPC_PLOC", "Loco session report");
    populate_both(OPC_NAME, "OPC_NAME", "Module name response");
    populate_both(OPC_STAT, "OPC_STAT", "Command station status report");
    populate_both(OPC_PARAMS, "OPC_PARAMS", "Node parameters response");
    populate_both(OPC_ACON3, "OPC_ACON3", "On event with 3 data bytes");
    populate_both(OPC_ACOF3, "OPC_ACOF3", "Off event with 3 data bytes");
    populate_both(OPC_ENRSP, "OPC_ENRSP", "Read node events response");
    populate_both(OPC_ARON3, "OPC_ARON3", "Accessory on response");
    populate_both(OPC_AROF3, "OPC_AROF3", "Accessory off response");
    populate_both(OPC_EVLRNI, "OPC_EVLRNI", "Teach event using event indexing");
    populate_both(OPC_ACDAT, "OPC_ACDAT", "Accessory data event: 5 bytes of node data (eg: RFID)");
    populate_both(OPC_ARDAT, "OPC_ARDAT", "Accessory data response");
    populate_both(OPC_ASON3, "OPC_ASON3", "Accessory short on with 3 data bytes");
    populate_both(OPC_ASOF3, "OPC_ASOF3", "Accessory short off with 3 data bytes");
    populate_both(OPC_DDES, "OPC_DDES", "Short data frame aka device data event (device id plus 5 data bytes)");
    populate_both(OPC_DDRS, "OPC_DDRS", "Short data frame response aka device data response");
    populate_both(OPC_ARSON3, "OPC_ARSON3", "Short response event on with 3 data bytes");
    populate_both(OPC_ARSOF3, "OPC_ARSOF3", "Short response event off with 3 data bytes");
    populate_both(OPC_EXTC6, "OPC_EXTC6", "Extended opcode with");
}
