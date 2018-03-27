#include <vector>
#include <boost/array.hpp>
#include <boost/bind.hpp>
#include <boost/asio/ip/udp.hpp>
#include <boost/asio/deadline_timer.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/thread.hpp>
#include <openssl/rand.h>
#include "util.h"
#include "net.h"
#include "endiannes.h"

using namespace boost::asio;

typedef boost::array<uint8_t, 20> bytes20;

struct MsgConnectRequest
{
    uint64BE connection_id;
    uint32BE action;         // 0 - connect
    uint32_t transaction_id;
};

struct MsgConnectResponse
{
    uint32BE action;         // 0 - connect
    uint32_t transaction_id;
    uint64_t connection_id;
};

struct MsgAnnounceRequest
{
    uint64_t connection_id;
    uint32BE action;         // 1 - announce
    uint32_t transaction_id;
    bytes20  info_hash;
    bytes20  peer_id;
    uint64BE downloaded;
    uint64BE left;
    uint64BE uploaded;
    uint32BE event;         // 0: none; 1: completed; 2: started; 3: stopped
    uint32BE IP_address;    // 0 - default
    uint32BE key;
    uint32BE num_want;      // -1 - default
    uint16BE port;
};

struct MsgAnnounceResponse
{
    uint32BE action;         // 1 - announce
    uint32_t transaction_id;
    uint32BE interval;
    uint32BE leechers;
    uint32BE seeders;
   // uint8_t peers[leechers + seeders][6];
};

class BTTracker
{
public:
    enum EStatus
    {
        CONNECTION_REQUESTED,
        ANNOUNCE_REQUESTED,
        STATUS_IDLE,
        CONNECTION_END_REQUESTED,
        STOPPED
    };

    ip::udp::endpoint m_Address;
    EStatus m_Status;
    int m_NumRequests;
    uint64_t m_LastTime;
    int m_WaitInterval;

    MsgConnectRequest m_ConnectRequest;
    MsgAnnounceRequest m_AnnounceRequest;

    BTTracker();
    void init(const ip::udp::endpoint &Address);
    void onSend(const boost::system::error_code& Error, std::size_t NumBytes);
    void onReceive(const uint8_t* pData, std::size_t Size);
    void onError(const boost::system::error_code& Error);
    void setStatus(EStatus Status);
    void sendConnectRequest(bool First);
    void sendAnnounceRequest(bool First, bool Start);
    void stop();
    void everySecond();
};

static bytes20 g_InfoHash = {{ 0x59, 0x8E, 0xAD, 0xC0, 0x19, 0x6F, 0x34, 0xB0, 0x73, 0xFD, 0xD4, 0xD1, 0xCA, 0x4C, 0x38, 0xBE, 0xCA, 0x22, 0xEF, 0x7E }};

static const int g_NumTrackers = 6;

static ip::udp::endpoint g_TrackerList[g_NumTrackers] =
{
    ip::udp::endpoint(ip::address_v4::from_string("31.172.63.253"),  80),   // udp://tracker.openbittorrent.com:80
    ip::udp::endpoint(ip::address_v4::from_string("192.121.121.30"), 80),   // udp://tracker.istole.it:80
    ip::udp::endpoint(ip::address_v4::from_string("194.71.107.27"),  80),   // udp://tracker.ccc.de:80
    ip::udp::endpoint(ip::address_v4::from_string("62.210.137.203"), 1337), // udp://open.demonii.com:1337
    ip::udp::endpoint(ip::address_v4::from_string("31.172.63.226"),  80),   // udp://tracker.publicbt.com:80
    ip::udp::endpoint(ip::address_v4::from_string("94.228.222.192"), 6969)  // udp://tracker.coppersurfer.tk:6969
};

static io_service g_IOService;
static ip::udp::socket g_Socket(g_IOService);
static ip::udp::endpoint g_RemoteEndpoint;
static boost::asio::deadline_timer g_Timer(g_IOService);
static boost::thread g_Thread;
static const int g_BufferSize = 1024;
static uint8_t g_Buffer[g_BufferSize];
static BTTracker g_Trackers[g_NumTrackers];
static bytes20 g_PeerID;

static std::string toString(ip::udp::endpoint Address)
{
    std::ostringstream StrStream;
    StrStream << Address;
    return StrStream.str();
}

BTTracker::BTTracker()
{}

void BTTracker::setStatus(EStatus Status)
{
    m_Status = Status;
    m_LastTime = GetTime();
}

void BTTracker::init(const ip::udp::endpoint& Address)
{
    m_Address = Address;

    // Init messages.
    m_ConnectRequest.connection_id = 0x41727101980;
    m_ConnectRequest.action = 0; // connect

    m_AnnounceRequest.action = 1; // announce
    m_AnnounceRequest.info_hash = g_InfoHash;
    m_AnnounceRequest.peer_id = g_PeerID;
    m_AnnounceRequest.downloaded = 10*1024*1024;
    m_AnnounceRequest.left       = 10*1024*1024;
    m_AnnounceRequest.uploaded   = 10*1024*1024;
    m_AnnounceRequest.IP_address = 0;
    m_AnnounceRequest.key = 0;
    m_AnnounceRequest.num_want = -1; // default
    m_AnnounceRequest.port = GetListenPort();

    sendConnectRequest(true);
}

void BTTracker::sendConnectRequest(bool First)
{
    if (First)
    {
        m_NumRequests = 0;
        RAND_bytes((uint8_t*)&m_ConnectRequest.transaction_id, 4);
    }
    else
        m_NumRequests++;

    printf("BTTrackers: connecting(%i) to %s\n", m_NumRequests, toString(m_Address).c_str());
    g_Socket.async_send_to(buffer(&m_ConnectRequest, 16), m_Address, boost::bind(&BTTracker::onSend, this, _1, _2));
    setStatus(CONNECTION_REQUESTED);
}

void BTTracker::stop()
{
    if (m_Status != ANNOUNCE_REQUESTED && m_Status != STATUS_IDLE)
        return;

    RAND_bytes((uint8_t*)&m_ConnectRequest.transaction_id, 4);

    printf("BTTrackers: connecting to %s to stop\n", toString(m_Address).c_str());
    g_Socket.async_send_to(buffer(&m_ConnectRequest, 16), m_Address, boost::bind(&BTTracker::onSend, this, _1, _2));
    setStatus(CONNECTION_END_REQUESTED);
}

void BTTracker::sendAnnounceRequest(bool First, bool Start)
{
    if (First)
    {
        m_NumRequests = 0;
        RAND_bytes((uint8_t*)&m_AnnounceRequest.transaction_id, 4);
    }
    else
        m_NumRequests++;

    m_AnnounceRequest.event = Start? 2 : 3; // started ot stopped

    if (Start)
        printf("BTTrackers: announcing(%i) on %s\n", m_NumRequests, toString(m_Address).c_str());
    else
        printf("BTTrackers: stopping on %s\n", toString(m_Address).c_str());
    g_Socket.async_send_to(buffer(&m_AnnounceRequest, 98), m_Address, boost::bind(&BTTracker::onSend, this, _1, _2));

    setStatus(Start? ANNOUNCE_REQUESTED : STOPPED);
}

void BTTracker::onSend(const boost::system::error_code& Error, std::size_t NumBytes)
{}

void BTTracker::onError(const boost::system::error_code& Error)
{}

static void addPeers(const std::string& Source, const uint8_t Peers[], int NumPeers)
{
    std::vector<CAddress> vAdd;
    for (int i = 0; i < NumPeers; i++)
    {
        int nOneDay = 24*3600;
        in_addr Addr;
        memcpy(&Addr.s_addr, &Peers[6*i], 4);
        CAddress addr = CAddress(CService(Addr, Peers[6*i + 4]*256 + Peers[6*i + 5]));
        addr.nTime = GetTime() - 3*nOneDay - GetRand(4*nOneDay); // use a random age between 3 and 7 days old
        vAdd.push_back(addr);
    }
    addrman.Add(vAdd, CNetAddr(Source, true));
}

void BTTracker::onReceive(const uint8_t* pData, std::size_t Size)
{
    switch (m_Status)
    {
    case CONNECTION_REQUESTED:
        {
            if (Size < 16)
                return;

            MsgConnectResponse& Response = *(MsgConnectResponse*)pData;
            if (Response.action != 0 || Response.transaction_id != m_ConnectRequest.transaction_id)
                return;

            printf("BTTrackers: connected to %s\n", toString(m_Address).c_str());
            m_AnnounceRequest.connection_id = Response.connection_id;
            sendAnnounceRequest(true, true);
        }
        break;

    case ANNOUNCE_REQUESTED:
        {
            if (Size < 20)
                return;

            MsgAnnounceResponse& Response = *(MsgAnnounceResponse*)pData;
            if (Response.action != 1 || Response.transaction_id != m_AnnounceRequest.transaction_id)
                return;

            int NumPeers = std::min((Size - 20)/6, size_t(Response.seeders + Response.leechers));
            addPeers(m_Address.address().to_string(), pData + 20, NumPeers);
            std::string Peers;
            char Buffer[32];
            for (int i = 0; i < std::min(NumPeers, 10); i++)
            {
                const uint8_t* pIP = pData + 20 + i*6;
                const uint8_t* pPort = pData + 20 + i*6 + 4;
                sprintf(Buffer, "; %i.%i.%i.%i:%i", pIP[0], pIP[1], pIP[2], pIP[3], pPort[0]*256 + pPort[1]);
                Peers += Buffer;
            }
            m_WaitInterval = Response.interval;
            printf("BTTrackers: announced on %s, next announce in %i seeconds, got %i peers: %s\n", toString(m_Address).c_str(), m_WaitInterval, NumPeers, Peers.c_str());
            setStatus(STATUS_IDLE);
        }
        break;

    case CONNECTION_END_REQUESTED:
    {
        if (Size < 16)
            return;

        MsgConnectResponse& Response = *(MsgConnectResponse*)pData;
        if (Response.action != 0 || Response.transaction_id != m_ConnectRequest.transaction_id)
            return;

        printf("BTTrackers: connected to %s to stop\n", toString(m_Address).c_str());
        m_AnnounceRequest.connection_id = Response.connection_id;
        sendAnnounceRequest(true, false);
    }
    break;

    default: // we are not expecting any data
        break;
    }
}

void BTTracker::everySecond()
{
    switch (m_Status)
    {
    case CONNECTION_REQUESTED:
        if (int(GetTime() - m_LastTime) > 15*(1 << m_NumRequests))
        {
            if (m_NumRequests <= 8)
                sendConnectRequest(false);
            else
            {
                m_WaitInterval = 90*60;
                setStatus(STATUS_IDLE);
            }
        }
        break;

    case ANNOUNCE_REQUESTED:
        if (int(GetTime() - m_LastTime) > 15*(1 << m_NumRequests))
        {
            if (m_NumRequests <= 3)
                sendAnnounceRequest(false, true);
            else
                sendConnectRequest(false);
        }
        break;

    case STATUS_IDLE:
        if ((uint64_t)GetTime() > m_LastTime + std::max(m_WaitInterval, 5*60))
            sendConnectRequest(true);
        break;

    default: // nothing to do
        break;
    }
}

static void startListening();
static void startTimer();

static void onReceive(const boost::system::error_code& Error, std::size_t NumBytes)
{
    for (int i = 0; i < g_NumTrackers; i++)
    {
        BTTracker& Tracker = g_Trackers[i];
        if (g_RemoteEndpoint == Tracker.m_Address)
        {
            if (!Error)
                Tracker.onReceive(g_Buffer, NumBytes);
            else
                Tracker.onError(Error);
            break;
        }
    }
    startListening();
}

static void startListening()
{
    g_Socket.async_receive_from(buffer(g_Buffer, g_BufferSize), g_RemoteEndpoint, onReceive);
}

static void everySecond(const boost::system::error_code& error)
{
    for (int i = 0; i < g_NumTrackers; i++)
        g_Trackers[i].everySecond();

    startTimer();
}

static void startTimer()
{
    g_Timer.expires_from_now(boost::posix_time::seconds(1));
    g_Timer.async_wait(everySecond);
}

static void sendStop()
{
    for (int i = 0; i < g_NumTrackers; i++)
        g_Trackers[i].stop();
}

void startBTTrackers()
{
    if (fTestNet)
        return;
    try
    {
        RAND_bytes(g_PeerID.begin(), 20);

        boost::system::error_code Error;
        g_Socket.open(ip::udp::v4());
        g_Socket.bind(ip::udp::endpoint(ip::udp::v4(), 0), Error);
        if (Error)
        {
            printf("BTTrackers, ERROR: can't bind UDP socket.\n");
            return;
        }

        for (int i = 0; i < g_NumTrackers; i++)
            g_Trackers[i].init(g_TrackerList[i]);

        startListening();
        startTimer();

        g_Thread = boost::thread(boost::bind((size_t (io_service::*)())&io_service::run, &g_IOService));
    }
    catch(...)
    {
        printf("BTTrackers, ERROR: exception.\n");
    }
}

void stopBTTrackers()
{
    g_IOService.dispatch(sendStop);
    MilliSleep(500); // give some time to establish connection
    g_IOService.stop();
    g_Thread.join();
}
