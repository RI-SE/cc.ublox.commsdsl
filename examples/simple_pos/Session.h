#pragma once

#include <cstdint>
#include <vector>
#include <string>

#include <boost/array.hpp>

#include "common/boost_wrap.h"

#include "cc_ublox/Message.h"
#include "cc_ublox/message/NavPvt.h"
#include "cc_ublox/message/NavAtt.h"
#include "cc_ublox/frame/UbloxFrame.h"

namespace cc_ublox
{

namespace simple_pos    
{

class Session 
{
    using InMessage =
        cc_ublox::Message<
            comms::option::ReadIterator<const std::uint8_t*>,
            comms::option::Handler<Session> // Dispatch to this object
        >;

    using OutBuffer = std::vector<std::uint8_t>;
    using OutMessage =
        cc_ublox::Message<
            comms::option::IdInfoInterface,
            comms::option::WriteIterator<std::back_insert_iterator<OutBuffer> >,
            comms::option::LengthInfoInterface
        >;

    using InNavPvt = cc_ublox::message::NavPvt<InMessage>;
    using InNavAtt = cc_ublox::message::NavAtt<InMessage>;

public:
    Session(common::boost_wrap::io& io, const std::string& dev);
    ~Session();

    bool start();

    void handle(InNavAtt& msg);
    void handle(InNavPvt& msg);

    void handle(InMessage& msg);

private:

    using AllInMessages =
        std::tuple<
            InNavAtt,
            InNavPvt
        >;

    using Frame = cc_ublox::frame::UbloxFrame<InMessage, AllInMessages>;

    using SerialPort = boost::asio::serial_port;

    void performRead();
    void processInputData();
    void sendPosPoll();
    void sendMessage(const OutMessage& msg);
    void configureUbxOutput();

    common::boost_wrap::io& m_io;
    SerialPort m_serial;
    boost::asio::deadline_timer m_pollTimer;
    std::string m_device;
    boost::array<std::uint8_t, 512> m_inputBuf;
    std::vector<std::uint8_t> m_inData;
    Frame m_frame;
};

} // namespace simple_pos

} // namespace cc_ublox
