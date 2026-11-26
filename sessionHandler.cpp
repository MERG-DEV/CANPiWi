#include "sessionHandler.h"

namespace {
/*
 * Holds a mutex for as long as it exists, so the mutex is released however the
 * block that made it is left, including by an exception.
 */
class mutexLock
{
    public:
        explicit mutexLock(pthread_mutex_t *mutex) : mutex(mutex) { pthread_mutex_lock(mutex); }
        ~mutexLock() { pthread_mutex_unlock(mutex); }
        mutexLock(const mutexLock&) = delete;
        mutexLock& operator=(const mutexLock&) = delete;
    private:
        pthread_mutex_t *mutex;
};
}

sessionHandler::sessionHandler(log4cpp::Category *logger,nodeConfigurator *config, canHandler *can)
{
    //ctor
    this->logger = logger;
    this->config = config;
    this->can = can;
    sessionids = 0;
    pthread_mutex_init(&m_mutex_in, NULL);
}

sessionHandler::~sessionHandler()
{
    //dtor
    this->sessions.clear();
    pthread_mutex_destroy(&m_mutex_in);
}


edSession* sessionHandler::createEDSession(int client_id, string edname, long client_ip){
    logger->debug("[sessionHandler] Allocate a new session for ed %s %d and ip %d", edname.c_str(), client_id, client_ip);
    //the session list is shared by the client threads and the keep alive thread
    mutexLock lock(&m_mutex_in);

    sessionids++;
    edSession *ed = new edSession(logger,sessionids);
    ed->setNodeConfigurator(config);
    ed->getMomentaryFNs();
    ed->setEdName(edname);
    ed->setClientIP(client_ip);
    ed->setClientId(client_id);
	ed->setOrphan(false);
    sessions.push_back(ed);

    return ed;
}

bool sessionHandler::deleteEDSession(int sessionuid, int client_id){
	logger->debug("[sessionHandler] Deleting session %d", sessionuid);
    //the session list is shared by the client threads and the keep alive thread
    mutexLock lock(&m_mutex_in);

    std::vector<edSession*>::iterator it = sessions.begin();
    while (it != sessions.end()){
        edSession *ed = *it;
        if (ed->getSessionUid() == sessionuid){
            if (ed->getClientId() != client_id){
                //taken over by another connection from the same address, which uses it now
                logger->debug("[sessionHandler] Session %d now belongs to client %d, not deleted", sessionuid, ed->getClientId());
                return false;
            }
            delete (ed);
            sessions.erase(it);
            return true;
        }
        it++;
    }



    return false;
}

unsigned int sessionHandler::retrieveAllEDSession(int client_id, string edname, long client_ip, vector<edSession*> *edsessions){
    unsigned int i=0;
	edSession *ed;
    //the session list is shared by the client threads and the keep alive thread
    mutexLock lock(&m_mutex_in);
    logger->debug("[sessionHandler] Checking for all %d existing sessions for ed %s %d and ip %d",sessions.size(), edname.c_str(), client_id, client_ip);
	std::vector<edSession*>::iterator it = sessions.begin();
    while (it != sessions.end()){
		ed = *it;
        //if (sessions[i]->getClientIP() == client_ip && sessions[i]->getEdName() == edname){
		logger->debug("[sessionHandler] Comparing ip %d and %d",ed->getClientIP(), client_ip);
		//a session still waiting for a loco belongs to its own client, which deletes it
		if (ed->getClientIP() == client_ip && ed->isSessionSet()){
            ed->setOrphan(false);
            ed->setClientId(client_id); //taken over: this client owns it now
            logger->debug("[sessionHandler] Retrieved session for loco %d", ed->getLoco());
            edsessions->push_back(ed);
			i++;
        }
		it++;
    }

	logger->debug("[sessionHandler] Found %d existing sessions for %s %d", i, edname.c_str(), client_id);

    return i;
}

bool sessionHandler::deleteAllEDSessions(int client_id){
	bool found = false;

    //the session list is shared by the client threads and the keep alive thread
    mutexLock lock(&m_mutex_in);

    std::vector<edSession*>::iterator it = sessions.begin();
    while (it != sessions.end()){
        edSession *ed = *it;
        if (ed->getClientId() == client_id){
            //erase returns the next session; the iterator erased can't be used again
            it = sessions.erase(it);
            delete (ed);
            found = true;
        }
        else{
            it++;
        }
    }
    return found;
}

void sessionHandler::sendKeepAliveForOrphanSessions(){
    long millis;
    struct timespec spec;
    struct timespec t;
    edSession *ed;
    try{
        //the session list is shared by the client threads and the keep alive thread
        mutexLock lock(&m_mutex_in);
		//logger->info("[sessionHandler] Sessions in progress %d", sessions.size());
        if (sessions.size() > 0){

            std::vector<edSession*>::iterator it = sessions.begin();
            while(it != sessions.end())
            {
				ed = *it;
                if (ed->isOrphan()){
                    clock_gettime(CLOCK_REALTIME,&spec);

                    /*
                     * check if has elapsed more than n seconds after the session became orphan
                     * If so delete the session
                    */

                    t = ed->getEDTime();//the last ed time will contain the last ed keep alive
                    millis = elapsed_millis(spec, t);//spec.tv_sec*1000 + spec.tv_nsec/1.0e6 - t.tv_sec*1000 - t.tv_nsec/1.0e6;
                    if (millis > (timeout_orphan * 1000 )){
                        //delete the session
                        logger->debug("[sessionHandler] Orphan ed timedout session:%d loco:%d. Deleting.",ed->getSessionUid(), ed->getLoco());
                        //set speed to 0
                        sendCbusMessage(OPC_DSPD, ed->getSession(), ed->getDirection() * BS);
                        //release session
                        sendCbusMessage(OPC_KLOC, ed->getSession());
                        //erase returns the next session; the iterator erased can't be used again
                        it = sessions.erase(it);
                        continue;
                    }

                    if (ed->getLoco() > -1 && ed->isSessionSet()){
                        t = ed->getCbusTime();
                        millis = elapsed_millis(spec, t);// spec.tv_sec*1000 + spec.tv_nsec/1.0e6 - t.tv_sec*1000 - t.tv_nsec/1.0e6;
                        if (millis > CBUS_KEEP_ALIVE ){
                            ed->setCbusTime(spec);
                            //send keep alive
                            logger->debug("[sessionHandler] Send CBUS keep alive loco [%d] session [%d]",ed->getLoco(),ed->getSession());
                            sendCbusMessage(OPC_DKEEP,ed->getSession());
                        }
                    }
                    else{
                        logger->debug("[%d] [sessionHandler] Loco not set keep alive",ed->getLoco());
                    }
                }
                it++;
            }
        }
    }//try
    catch(...){
        logger->debug("[sessionHandler] Keep alive error");
    }
}

void sessionHandler::sendCbusMessage(byte b0, byte b1){
    char msg[CAN_MSG_SIZE];
    msg[0] = b0;
    msg[1] = b1;
    logger->debug("[sessionHandler] Sending message to CBUS");
    can->put_to_out_queue(msg,2,CLIENT_TYPE::ED);
}

void sessionHandler::sendCbusMessage(byte b0, byte b1, byte b2){
    char msg[CAN_MSG_SIZE];
    msg[0] = b0;
    msg[1] = b1;
    msg[2] = b2;
    logger->debug("[sessionHandler] Sending message to CBUS");
    can->put_to_out_queue(msg,3,CLIENT_TYPE::ED);
}

void sessionHandler::start(){
    running = 1;
	timeout_orphan = config->getOrphanTimeout();
    pthread_create(&sessionHandlerThread, nullptr, sessionHandler::run_thread_entry, this);
}

void sessionHandler::stop(){
    running = 0;
}

void sessionHandler::run(void *param){
    while (running){
        try{
            usleep(1000*500);//500ms
            if (running == 0){
                logger->info("[sessionHandler] Stopping keep alive process");
                break;
            }
            sendKeepAliveForOrphanSessions();
        }
        catch(...){
            logger->debug("[sessionHandler] Run process error");
        }
    }

    logger->info("[sessionHandler] Finish keep alive process for orphan sessions");
}
